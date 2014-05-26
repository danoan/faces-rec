#include "../../headers/classifier/directory.h"

bool isFile(std::string path){
    return (opendir(path.c_str()) == NULL);
}

int getAllDirectories(std::string path, std::vector<std::string>& dirs){
    DIR* dir;
    struct dirent *ent;    

    if( (dir = opendir(path.c_str()) ) != NULL){        
        while( (ent=readdir(dir))!=NULL ){
            if( strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0 ) continue;


            if( !isFile(path + "/" + ent->d_name) ){
                dirs.push_back( (path + "/" + ent->d_name) );
            }

        }
        closedir(dir);
    }else{
        printf("Directory could not be opened\n");
        return -1;
    }          
}

int getFiles(std::string path, int n, std::vector<std::string>& files){
     DIR* dir;
    struct dirent *ent;    

    // printf("%s\n",path.c_str());
    if( (dir = opendir(path.c_str()) ) != NULL){        
        while( (ent=readdir(dir))!=NULL ){
            if( strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0 ) continue;

            if(n!=-1){
                // printf("BREAK! %d\n",files.size());
                if(n<=files.size()) break;
            }

            // printf("%s\n",(path + "/" + ent->d_name).c_str());
            if( isFile(path + "/" + ent->d_name) ){
                files.push_back( (path + "/" + ent->d_name) );
            }else{
                getFiles( (path + "/" + ent->d_name), n, files);
            }

        }
        closedir(dir);
    }else{
        printf("Directory could not be opened\n");
        return -1;
    }             
}

int getAllFiles(std::string path, std::vector<std::string>& files){
    getFiles(path,-1,files);
    return 1;
}
