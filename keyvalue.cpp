#include<bits/stdc++.h>
#include <sys/stat.h>
#define maxsize 5000000

using namespace std;

int findposition(FILE *tmp, unsigned long long int num){
    unsigned long long int disk_num;
    bool find=false;
    int pos=0;
    fseek(tmp, 0, SEEK_SET);
    while(fread (&disk_num, sizeof(unsigned long long int), 1, tmp)){  
        if(num==disk_num){
        // cout<<disk_num<<endl;
            find=true;
            break;
        }
        fseek(tmp, 128, SEEK_CUR);
        pos+=1;
    }
    if(!find){
        return -1;
    }
    else{
        // cout<<"Iii"<<endl;
        return pos;
    }
    
}

void printdisk(FILE* tmp){
    unsigned long long int disk_num;
    char* mem = new char[129];
    cout<<"in the disk: "<<endl;
    fseek(tmp, 0, SEEK_SET);
        while(fread (&disk_num, sizeof(unsigned long long int), 1, tmp))  {
            fread(mem,sizeof(char),128,tmp);
            
            cout<<disk_num<<" ";
            cout<<mem<<endl;

        }
        cout<<"=================\n";
        delete[] mem;
}
void printindex(FILE* tmp){
    unsigned long long int disk_num;
    int n;
    cout<<"in the index: "<<endl;
    fseek(tmp, 0, SEEK_SET);
    fread(&n,sizeof(int),1,tmp);
    cout<<"count: "<<n<<endl;
    while(fread(&disk_num, sizeof(unsigned long long int), 1, tmp))  {
        fread(&n,sizeof(int),1,tmp);
        cout<<disk_num<<" ";
        cout<<n<<endl;

        }
    cout<<"=================\n";
}
void printkey(FILE* tmp){
    unsigned long long int disk_num;
    int n;
    cout<<"in the key: "<<endl;
    fseek(tmp, 0, SEEK_SET);
        while(fread(&disk_num, sizeof(unsigned long long int), 1, tmp))  {
            cout<<disk_num<<endl;
        }
        cout<<"=================\n";
}
int main(int argc, char *argv[]){
    list<unsigned long long int>storage_key;
    unordered_map<unsigned long long int ,pair<string, list<unsigned long long int>::iterator > >storage;
    list<unsigned long long int>::iterator iter;
    unordered_map<unsigned long long int, int>indexbox;
    stringstream ss,filename;
    string word,key, value,disk_value,disk_key;
    unsigned long long int num,num1,disk_num,n;
    char s[153],s_disk[150];
    const char *delim = ".";
    char * pch;
    bool check=false,isempty=false;
    int len=0,indexnum=50000000;
    char* mem ;
    int count=0;
    int pos=0;
    bool output=false;
    mkdir("./storage", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    FILE *in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("Error while opening the input file.\n");
        exit(EXIT_FAILURE);
    }
    pch = strtok(argv[1],delim);
    filename<<pch;
    filename<<".output";
    FILE *out = fopen(filename.str().c_str(), "w");

    FILE *temp=fopen("storage/disk.tmp","r+");
    if (!temp) {
        temp=fopen("storage/disk.tmp","w+");
    }
    
    FILE *input_index= fopen("storage/index.tmp", "a+");
    if (!input_index) {
        fclose(input_index);
    }       
    else{
        unsigned long long int num;
        int pos;
        fseek(input_index,0,SEEK_SET);
        fread(&count,sizeof(int),1,input_index);
        while(fread(&num, sizeof(unsigned long long int), 1, input_index)){            
            fread(&pos,sizeof(int),1, input_index);
            indexbox[num]=pos;
        }       
    }
    // FILE *input_key = fopen("storage/key.tmp", "a+");
    // if (!input_key) {
    //     fclose(input_key);
    // }
    // else{
    //     unsigned long long int num;
    //     char* mem=new char[129];
    //     int pos;
        // fseek(input_key,0,SEEK_SET);
        // fseek(temp,0,SEEK_SET);
        // while(fread(&num, sizeof(unsigned long long int), 1, input_key)){
        //     pos=findposition(temp,num);
        //     fseek(temp,pos*136+8,SEEK_SET);
        //     fread(mem,sizeof(char),128,temp);
        //     // cout<<"out: "<<num<<" "<<mem<<endl;
        //     storage_key.push_front(num);
        //     storage[num]=make_pair(mem,storage_key.begin());
        //     if(storage_key.size()==maxsize){
        //         break;
        //     }
        // }
        // delete[] mem;
    // }      
    // printdisk(temp);
    // cout<<"in the map"<<endl;
    // for(unordered_map<unsigned long long int ,pair<string, list<unsigned long long int>::iterator > > ::iterator it = storage.begin();
    // it != storage.end(); ++it)
    // {
    // std::cout << it->first << " " << it->second.first.c_str() << "\n";
    // }
    // cout<<"in the index"<<endl;
    // for(unordered_map<unsigned long long int ,int > ::iterator it = indexbox.begin();
    // it != indexbox.end(); ++it)
    // {
    // std::cout << it->first << " " << it->second << "\n";
    // }

    while( fgets(s, 153, in)!=NULL){
        string str(s);
        istringstream iss(str);
        iss>>word;
        iss>>key;
        iss>>value;
        num=atoll(key.c_str());
        // cout<<"PUT "<<num<<" "<<value.c_str()<<endl;
        if(word=="PUT"){  
            if(storage.find(num)==storage.end()){//不在storage
                if(storage_key.size()==maxsize){//storage滿
                        // cout<<"PUT:"<<endl;   
                    if(indexbox.find(num)!=indexbox.end()){//先查index
                            // cout<<"yp";
                            pos=indexbox.find(num)->second;
                            fseek(temp, pos*136+8, SEEK_SET);
                            string *str=new string();
                            *str=value;
                            fwrite((*str).c_str(),sizeof(char),128,temp);//有直接改

                    }
                    else{//看看有沒有在disk
                        pos=findposition(temp,num);
                        if(pos!=-1){//有在disk
                            fseek(temp, pos*136+8, SEEK_SET);
                            string *str=new string();
                            *str=value;
                            fwrite((*str).c_str(),sizeof(char),128,temp);//有直接改
                        }
                    }
                    auto data = storage_key.back();//準備被pop的數
                    n=storage.find(data)->first;
                    // cout<<n<<endl;
                    pos=findposition(temp,n);
                    string *str=new string();
                    *str=storage.find(data)->second.first;
                    // cout<<"pop: "<<str->c_str()<<endl;
                    
                    // cout<<"pos: "<<pos<<endl;
                    if(pos!=-1){
                        fseek(temp, pos*136+8, SEEK_SET);
                        fwrite((*str).c_str(),sizeof(char),128,temp);//有直接改
                    }
                    else{
                        if(indexbox.size()==indexnum){
                            auto it = indexbox.begin();
                            srand(time(NULL));
                            advance(it, rand() % indexbox.size());
                            // cout<<"push erase: "<<it->first<<endl;
                            indexbox.erase(it);
                        }
                        indexbox[n]=count;
                        // cout<<"存"<<n<<" "<<count<<endl;
                        
                        fseek(temp,count*136,SEEK_SET);//最後新增
                        fwrite(&n,sizeof(unsigned long long),1,temp);
                        fwrite((*str).c_str(),sizeof(char),128,temp);
                        count++;
                    }

                    // fseek(temp,len,SEEK_SET);
                    // // int len1=ftell(temp);
                    // // cout<<len1<<endl;
                    // fread(&disk_num,sizeof(unsigned long long),1,temp);
                    
                    // char* mem = (char *) malloc(sizeof(char)*128);
                    // fread(mem,sizeof(char),128,temp);
                    // cout<<disk_num<<endl;
                    // cout<<mem;
                    
                    storage.erase(data);
                    storage_key.pop_back();
                }
                    storage_key.push_front(num);
                    storage[num]=make_pair(value,storage_key.begin());
            }
            else{
                storage_key.erase(storage[num].second);
                storage_key.push_front(num);
                storage[num]=make_pair(value,storage_key.begin());
            }
        }
        else if(word=="GET"){
            output=true;
           auto iter = storage.find(num);
            if(iter!=storage.end()){//在storage
            // cout<<"hehe"<<endl;
                value=iter->second.first;
                fputs(iter->second.first.c_str(),out);
                fputs("\n",out);
                storage_key.erase(iter->second.second);
                storage_key.push_front(num);  
                storage[num]=make_pair(value,storage_key.begin());     
            }
            else//不在storage
            {
                if(indexbox.size()==0){//disk沒東西直接empty
                    fputs("EMPTY\n",out);
                    isempty=true;
                    // cout<<"yo"<<endl;
                }
                else if(indexbox.find(num)!=indexbox.end()){//先查index
                        pos=indexbox.find(num)->second;
                }
                else{
                    pos=findposition(temp,num);
                    // cout<<pos<<endl;
                    if(pos==-1){//沒在disk
                        fputs("EMPTY\n",out);
                        isempty=true;
                    }
                    // cout<<"yaya"<<endl;
                }     
                if(!isempty){
                    // cout<<"aaaaaaaaaaa"<<endl;
                    fseek(temp, pos*136+8, SEEK_SET);
                    char* mem = new char[129];
                    fread(mem,sizeof(char),128,temp);
                    // cout<<"out: "<<mem<<endl;
                    fputs(mem,out);
                    fputs("\n",out);
                    auto data = storage_key.back();
                    n=storage.find(data)->first;
                    string *str=new string();
                    *str=storage.find(data)->second.first;
                    pos=findposition(temp,n);
                    if(pos!=-1){
                        fseek(temp, pos*136+8, SEEK_SET);
                        fwrite((*str).c_str(),sizeof(char),128,temp);//有直接改
                    }
                    else{
                        
                        if(indexbox.size()==indexnum){
                            
                            auto it = indexbox.begin();
                            srand(time(NULL));
                            advance(it, rand()%indexbox.size());
                            // cout<<"push erase: "<<it->first<<endl;
                            indexbox.erase(it);
                        }
                        indexbox[n]=count;
                        // cout<<"存"<<n<<" "<<count<<endl;
                        
                        fseek(temp,count*136,SEEK_SET);//最後新增
                        fwrite(&n,sizeof(unsigned long long),1,temp);
                        fwrite((*str).c_str(),sizeof(char),128,temp);
                        count++;
                    }
                    storage.erase(data);
                    storage_key.pop_back();
                    storage_key.push_front(num); 
                    storage[num]=make_pair(mem,storage_key.begin());
                    delete[] mem;
                }
                isempty=false;     
            }
            
        } 

        else if(word=="SCAN"){
            output=true;
            num1=atoll(value.c_str());
            for(unsigned long int i=num;i<=num1;i++){
                auto iter = storage.find(i);
                if(iter!=storage.end()){
                    value=iter->second.first;
                    fputs(iter->second.first.c_str(),out);
                    fputs("\n",out);
                    storage_key.erase(iter->second.second);
                    storage_key.push_front(i);          
                    storage[i]=make_pair(value,storage_key.begin());        
                }
                else
                {
                    if(indexbox.size()==0){//disk沒東西直接empty
                        fputs("EMPTY\n",out);
                        isempty=true;
                    }
                    else if(indexbox.find(i)!=indexbox.end()){//先查index
                        pos=indexbox.find(i)->second;
                        // cout<<"pos: "<<pos<<endl;
                    }
                    else{
                        pos=findposition(temp,i);
                        if(pos==-1){//沒在disk
                            fputs("EMPTY\n",out);
                            isempty=true;
                        }
                    }
                    if(!isempty){
                        fseek(temp, pos*136+8, SEEK_SET);
                        char* mem = new char[129];
                        fread(mem,sizeof(char),128,temp);
                            // cout<<"out: "<<mem<<endl;
                        fputs(mem,out);
                        fputs("\n",out);
                        auto data = storage_key.back();
                        n=storage.find(data)->first;
                        // cout<<n<<endl;
                        string *str=new string();
                        *str=storage.find(data)->second.first;
                        pos=findposition(temp,n);
                        // cout<<"pos: "<<pos<<endl;
                        if(pos!=-1){
                            fseek(temp, pos*136+8, SEEK_SET);
                            fwrite((*str).c_str(),sizeof(char),128,temp);//有直接改
                        }
                        else{
                            if(indexbox.size()==indexnum){
                                
                                auto it = indexbox.begin();
                                srand(time(NULL));
                                advance(it, rand()%indexbox.size());
                                // cout<<"push erase: "<<it->first<<endl;
                                indexbox.erase(it);
                            }
                            indexbox[n]=count;
                            // cout<<"存"<<n<<" "<<count<<endl;
                            
                            fseek(temp,count*136,SEEK_SET);//最後新增
                            fwrite(&n,sizeof(unsigned long long),1,temp);
                            fwrite((*str).c_str(),sizeof(char),128,temp);
                            count++;

                        }
                        storage.erase(data);
                        storage_key.pop_back();
                        storage_key.push_front(i); 
                        storage[i]=make_pair(mem,storage_key.begin());
                        delete[] mem;
                    }
                    isempty=false;
                }
            }
        }
    
    // cout<<"in the index"<<endl;
    // for(unordered_map<unsigned long long int ,int > ::iterator it = indexbox.begin();
    // it != indexbox.end(); ++it)
    // {
    //     cout << it->first << " " << it->second << "\n";
    // }
    }
    // cout<<"in the map"<<endl;
    // for(unordered_map<unsigned long long int ,pair<string, list<unsigned long long int>::iterator > > ::iterator it = storage.begin();
    // it != storage.end(); ++it)
    // {
    // std::cout << it->first << " " << it->second.first.c_str() << "\n";
    // }
    //     cout<<"in the index"<<endl;
    // for(unordered_map<unsigned long long int ,int > ::iterator it = indexbox.begin();
    // it != indexbox.end(); ++it)
    // {
    //     cout << it->first << " " << it->second << "\n";
    // }
    if(!output){
        remove(filename.str().c_str());
    }
    for(auto iter=storage.begin();iter!=storage.end();iter++){
        n=iter->first;
        // cout<<n<<endl;
        pos=findposition(temp,n);
        string *str=new string();
        *str=iter->second.first;
                    // cout<<"pop: "<<str->c_str()<<endl;
                    
                    // cout<<"pos: "<<pos<<endl;
        if(pos!=-1){
            fseek(temp, pos*136+8, SEEK_SET);
            fwrite((*str).c_str(),sizeof(char),128,temp);//有直接改
        }
        else{
            if(indexbox.size()==indexnum){
                auto it = indexbox.begin();
                srand(time(NULL));
                advance(it, rand() % indexbox.size());
                // cout<<"push erase: "<<it->first<<endl;
                indexbox.erase(it);
            }
            indexbox[n]=count;
            // cout<<"存"<<n<<" "<<count<<endl;
                        
            fseek(temp,count*136,SEEK_SET);//最後新增
            fwrite(&n,sizeof(unsigned long long),1,temp);
            fwrite((*str).c_str(),sizeof(char),128,temp);
            count++;
        }
    }
    // FILE *key_file=fopen("storage/key.tmp","r+");
    // if (!key_file) {
    //     key_file = fopen("storage/key.tmp", "w+");
    // }
    // fseek(key_file,0,SEEK_SET);
    // for(auto iter=storage.begin();iter!=storage.end();iter++){
    //     fwrite(&(iter->first) , sizeof(unsigned long long), 1, key_file);
    //     n=iter->first;
    //     // cout<<n<<endl;
    //     pos=findposition(temp,n);
    //     string *str=new string();
    //     *str=iter->second.first;
    //                 // cout<<"pop: "<<str->c_str()<<endl;
                    
    //                 // cout<<"pos: "<<pos<<endl;
    //     if(pos!=-1){
    //         fseek(temp, pos*136+8, SEEK_SET);
    //         fwrite((*str).c_str(),sizeof(char),128,temp);//有直接改
    //     }
    //     else{
    //         if(indexbox.size()==indexnum){
    //             auto it = indexbox.begin();
    //             srand(time(NULL));
    //             advance(it, rand() % indexbox.size());
    //             // cout<<"push erase: "<<it->first<<endl;
    //             indexbox.erase(it);
    //         }
    //         indexbox[n]=count;
            // cout<<"存"<<n<<" "<<count<<endl;
                        
        //     fseek(temp,count*136,SEEK_SET);//最後新增
        //     fwrite(&n,sizeof(unsigned long long),1,temp);
        //     fwrite((*str).c_str(),sizeof(char),128,temp);
        //     count++;
        // }
        

    // }
    // printdisk(temp);
    // printkey(key_file);
    FILE *index_file = fopen("storage/index.tmp", "r+");
    if (!index_file) {
        index_file = fopen("storage/index.tmp", "w+");
    }
    fseek(index_file,0,SEEK_SET);
    fwrite(&count,sizeof(int),1,index_file);
    for ( unordered_map<unsigned long long int ,int > ::iterator  iter=indexbox.begin();iter!= indexbox.end();iter++)
    {
        // cout<<iter->first<< " "<<iter->second<<endl;
        fwrite(&(iter->first), sizeof(unsigned long long int), 1, index_file);
        fwrite(&(iter->second), sizeof(int), 1, index_file);
    }
    // printindex(index_file);
    fclose(in);
    fclose(out);
    fclose(temp);
    // fclose(key_file);
    fclose(index_file);
}
