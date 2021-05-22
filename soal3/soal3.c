#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h> //buat getcwd
#include <sys/stat.h>
#include <ctype.h>
#include<dirent.h>

char dest[1000];
char save[1000][1000];
int indeks=0;

int isRegular(const char *path){ //cek file/dir
    struct stat path_stat;
    stat(path,&path_stat);
    return S_ISREG(path_stat.st_mode);
}

//cek file ada atau ngga
int checkIfFileExists(const char * filename)
{
    FILE *file;
    file = fopen(filename, "rb");
    if (file!=NULL)
    {
        printf("%s\n",filename);
        fclose(file);
        if(isRegular(filename)) return 1; //cek file
        else return 0;
    }

    return 0;
}

//cek extension
void cekExt(char* fileName,char *ext){ 
    char *extt=strchr(fileName,'.'); //kalau ada 2 ext->ambil paling depan
    if(extt==fileName){
        strcpy(ext,"Hidden");
    }
    else if (extt==NULL){
        strcpy(ext,"Unknown");
    }
    else{
        strcpy(ext,extt+1);
        for(int x=0;x<strlen(ext);x++){
            //ubah jadi lowercase
            ext[x]=tolower(ext[x]);
        }
    }
}

int listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return 0;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            char destDir[1000];
            strcpy(destDir,basePath);
            strcat(destDir,"/");
            strcat(destDir,dp->d_name);
            printf("%s\n",destDir);
           
            if(checkIfFileExists(destDir)){
                strcpy(save[indeks],destDir);
                indeks+=1;
            }
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
        }
    }

    closedir(dir);
    return 1;
}

void *moveFile(void* argc){
    char* src=(char*) argc;

    //pindah file
    char file[1000];
    strcpy(file,src);
    //printf("%s",file);
       
    char *ndir,*dir;
    char x='/'; // home/usr/modul /soal.c
    char y='.';
    dir = strrchr(file,x); //yg pertamakali dijumpai
    ndir = strchr(file, y); 
    char ext[1000];
    //printf("%s",dir);
    if (dir){
        //ngecek file/dir/ada ngga
        if(checkIfFileExists(file)){
            cekExt(dir+1,ext);
        }
        else{
            return 0;
        }
    }
    mkdir(ext,0777);//extension dir
    //src
    char path[1000];
    strcpy(path,(char*) argc);
    //dst
    char fileCat[1000];
    getcwd(fileCat,sizeof(path));
    strcat(fileCat,"/");
    strcat(fileCat,ext);
    strcat(fileCat,"/");
    strcat(fileCat,dir+1);
    printf("%s\n%s\n",path,fileCat);
    rename(path,fileCat);
    
    return(void *) 1;
    //moves(src);
    pthread_exit(0);
}

int main(int argc, char* argv[]){
    
    char src[1000];
    //get dir
    getcwd(dest,sizeof(dest));
    strcpy(src,dest);

    pthread_t tid[argc-2];
    char dir[100];
    //-f membuat kategori file
    if(strcmp(argv[1],"-f")==0){
        int i=0;
        for(int x=2;x<argc;x++){
            //printf("%s", argv[x]);
            pthread_create(&tid[i],NULL,moveFile,(void *)argv[x]);
            i++;
        }
        for(int x=0;x<i;x++){
            void *ptr;
            pthread_join(tid[x],&ptr);
            
            if(((int) ptr)==1){
                printf("File %d : Berhasil Dikategorikan \n", x+1);
            }
            else{
                printf("File %d : Sad, gagal :( \n",x+1);
            }
        }
        return 0;
    }
    else if(strcmp(argv[1],"-d")==0){
        if(argc==3){ //bisa menerima 1 path
            strcpy(dir, argv[2]);
            
        }
    }
    else if(strcmp(argv[1],"*")==0){
        strcpy(dir,src);
    }

    if(!listFilesRecursively(dir)){
        printf("Yah, gagal disimpan :(\n");
        exit(0);
    }

    pthread_t tid2[indeks];
    printf("%d",indeks);
    int i=0,j=0;
    for(;i<indeks;i++){
        printf("%s\n",save[i]);
        pthread_create(&tid2[i],NULL,moveFile,(void *)save[i]);
    }
    for(;j<indeks;j++)
    {
        void *ptr;
        pthread_join(tid2[j],&ptr);
    }
    if(strcmp(argv[1],"-d")==0) printf("Berhasil disimpan\n");
    return 0;
}
