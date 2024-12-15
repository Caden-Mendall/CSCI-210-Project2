

#include "types.h"
#include "string.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){
    //check if the path is empty
    if(pathName[0]==0||!strcmp(pathName,"/")){
        printf("MKDIR ERROR: no path provided\n");
        return;
    }
    char baseName[64]={0};
    char dirName[256]={0};
    //call split path
    struct NODE* current=splitPath(pathName,baseName,dirName);
    //split path doesn't return, exit
    if(current==NULL){
        return;
    }
    
    struct NODE* searchNode=current->childPtr;
    //check if it already exists
    while(searchNode){
        if(!strcmp(searchNode->name,baseName)&&searchNode->fileType=='D'){
            printf("MKDIR ERROR: directory %s already exists\n",searchNode->name);
            return;
        }
        searchNode=searchNode->siblingPtr;
    }
    //make the new node
    struct NODE* newNode=(struct NODE*)malloc(sizeof(struct NODE));
    newNode->fileType='D';
    newNode->parentPtr=current;
    newNode->siblingPtr=newNode->childPtr=NULL;
    strcpy(newNode->name,baseName);

    //add the node into the structure
    if(!current->childPtr){
        current->childPtr=newNode;
        printf("MKDIR SUCCESS: node %s successfully created\n",pathName);
        return;
    }
    searchNode=current->childPtr;
    while(1){
        if(!searchNode->siblingPtr){
            searchNode->siblingPtr=newNode;
            printf("MKDIR SUCCESS: node %s successfully created\n",pathName);
            return;
        }
        searchNode=searchNode->siblingPtr;
    }
    //in theory, it should never reach this point
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    //check for no path name
    if(!pathName[0]){
        pathName[0]='/';
        pathName[1]=0;
    }
    baseName[0]=dirName[0]=0;
    
    char string[64]={0};
    int dirEnd=0;
    int length=0;
    //split the path into base and dir
    while(1){
        string[length]=pathName[dirEnd+length];
        if(string[length]=='/'){
            string[length+1]=0;
            strcpy(dirName+dirEnd,string);
            dirEnd+=(length+1);
            length=0;
        }else if(!string[length]){
            strcpy(baseName,string);
            break;
        }else{
            length++;
        }
    }
    //make sure dirName ends in a terminator
    if(dirEnd>0){
        dirName[dirEnd-1]=0;
    }else{
        dirName="";
    }
    
    int idx=0;
    struct NODE* current;
    if(pathName[0]=='/'){
        current=root;
        idx=1;
    }else{
        current=cwd;
    }
    length=0;
    char str[256]={0};
    if(dirName[0]==0){
        return current;
    }
    //go through the file system to find the node
    while(1){
        if(dirName[idx+length]=='/'||dirName[idx+length]==0){
            strcpy(str,dirName+idx);
            str[length]=0;
            //once a delimiter or terminator is found, traverse through the children to find it
            if(!current->childPtr){
                printf("ERROR: directory %s does not exist\n", str);
                return NULL;
            }
            current=current->childPtr;
            while(1){
                if(!strcmp(current->name,str)&&current->fileType=='D'){
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
        length++;
    }
    return NULL;
}
