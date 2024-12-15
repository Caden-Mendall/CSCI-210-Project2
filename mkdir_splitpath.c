

#include "types.h"
#include "string.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    if(pathName[0]==0||!strcmp(pathName,"/")){
        printf("MKDIR ERROR: no path provided");
        return;
    }
    char baseName[64]={0};
    char dirName[256]={0};
    struct NODE* current;
    current=splitPath(pathName,baseName,dirName);
    
    printf("1\n");
    
    struct NODE* searchNode=current->childPtr;
    //check if it already exists
    while(searchNode){
        if(!strcmp(searchNode->name,baseName)&&searchNode->fileType=='D'){
            printf("MKDIR ERROR: directory %s already exists\n",current->name);
            return;
        }
        searchNode=searchNode->siblingPtr;
    }
    struct NODE* newNode=(struct NODE*)malloc(sizeof(struct NODE));
    newNode->fileType='D';
    newNode->parentPtr=current;
    newNode->siblingPtr=newNode->childPtr=NULL;
    strcpy(newNode->name,baseName);

    if(!current->childPtr){
        current->childPtr=newNode;
        printf("MKDIR SUCCESS: node %s successfully created",pathName);
        return;
    }
    searchNode=current->childPtr;
    while(1){
        if(!searchNode->siblingPtr){
            searchNode->siblingPtr=newNode;
            printf("MKDIR SUCCESS: node %s successfully created",pathName);
            return;
        }
        searchNode=searchNode->siblingPtr;
    }

    printf("TO BE IMPLEMENTED\n");

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    if(!pathName[0]){
        pathName[0]='/';
        pathName[1]=0;
    }
    baseName[0]=dirName[0]=0;

    printf("%s\n",pathName);
    
    char string[64]={0};
    int dirEnd=0;
    int length=0;
    while(1){
        string[length]=pathName[dirEnd+length];
        //string[length+1]=0;
        
        if(string[length]=='/'){
            string[length+1]=0;
            strcpy(dirName+dirEnd,string);
            //printf("%s\n",dirName);
            /*
            for(int i=0;i<length;i++){
                dirName[dirEnd+i]=string[i];
            }*/
            dirEnd+=(length+1);
            length=0;
            //string[0]=0;
        }else if(!string[length]){
            //string[length+1]=0;
            strcpy(baseName,string);
            /*
            for(int i=0;i<length;i++){
                baseName[i]=string[i];
            }
            */
            break;
        }else{
            length++;
        }
    }
    
    
    if(dirEnd>0){
        dirName[dirEnd-1]=0;
    }else{
        dirName="";
    }
    
    //correct up to here
    printf("%s, %s\n",baseName,dirName);

    struct NODE* current;
    if(pathName[0]=='/'){
        current=root;
    }else{
        current=cwd;
    }
    int idx=0;
    length=0;
    char* str=NULL;
    while(1){
        if(dirName[idx+length]=='/'||dirName[idx+length]==0){
            
            printf("%s\n",dirName+idx);
            
            strcpy(str,dirName+idx);

            printf("%s\n",str);
            
            str[length]=0;
            if(!current->childPtr){
                printf("ERROR: directory %s does not exist\n", str);
                return NULL;
            }
            current=current->childPtr;
            while(1){
                if(current->name==str&&current->fileType=='D'){
                    if(dirName[idx+length]==0){
                        return current;
                    }
                    break;
                }
                if(!current->siblingPtr){
                    printf("ERROR: directory %s does not exist\n", str);
                    return NULL;
                }
                current=current->siblingPtr;
            }
            idx=idx+length+1;
            length=0;
        }
        
        printf("3\n");
        
        length++;
    }

    /*
    string=dirName;
    char[64] token=strtok(string,"/");
    length=0;
    while(string+length!=token){
        length++;
    }
    //length=token-string;
    while(1){
        
        token=strtok(0,"/");
        if(!token){
            break;
        }
        string=token+1;
    }
    */
    
    return NULL;
}
