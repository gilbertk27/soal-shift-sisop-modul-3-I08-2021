# soal-shift-sisop-modul-3-I08-2021
 
 
### NO. 2
#### 2 a.  Create a matrix multiplication program (4x3 and 3x6) and then display the results. The matrix will contain the numbers 1-20 (no need to create a number filter).

##### Explanation 2a

The multiplication between 4x3 and 3x6 matrix on 2a done so that it will result in 4x6 matrix. To do the calculation, we make a startcount thread which is decalred as follows.
```    
void* startcount (void *arg) {

    pthread_t id = pthread_self();

for (int i = 0; i < 24 ; i++){
  
  for (int j = 0; j < 6; j++){
  	if (pthread_equal(id, t_id[i])){      // baris 1 matriks c
  	C[0][j] = A[0][0]*B[0][j] + A[0][1]*B[1][j] + A[0][2]*B[2][j];
  	}
  	i++;
   }
	
   for (int j = 0; j < 6; j++){
    	if (pthread_equal(id, t_id[i])){      // baris 2 matriks c
    	C[1][j] = A[1][0]*B[0][j] + A[1][1]*B[1][j] + A[1][2]*B[2][j];
    	}
    	i++;
   }
   
    for (int j = 0; j < 6; j++){
    	if (pthread_equal(id, t_id[i])){      // baris 3 matriks c
    	C[2][j] = A[2][0]*B[0][j] + A[2][1]*B[1][j] + A[2][2]*B[2][j];
    	}
    	i++;
   }
   
    for (int j = 0; j < 6; j++){
    	if (pthread_equal(id, t_id[i])){      // baris 4 matriks c
    		C[3][j] = A[3][0]*B[0][j] + A[3][1]*B[1][j] + A[3][2]*B[2][j];
    	}
    	i++;
   }
```

Because the result of the matrix multiplication will be used on 2b, after done with the calculation, the result then saved in shared memory with code as follows.

```
key_t key = 1234;
    int shmid = shmget(key, sizeof(int)*row*col, IPC_CREAT | 0666);
    matrix = (int *)shmat(shmid, NULL, 0);

    for (i = 0; i < row; i++){
        for (j = 0; j < col; j++){
            matrix[i*col + j] = C[i][j];
            printf("%d\t", matrix[i*col + j]);
        }
        printf("\n");   
    }

    sleep(5);
    int *p = (int *) matrix;
    memcpy(p, C, 95);
    shmdt(matrix);
    shmctl(shmid, IPC_RMID, NULL);
```

Here's a brief explanation of the code above:
+ `shmid` will return the value after we call the system on `shmget(key, sizeof(int)*row*column, IPC_CREAT | 0666)` which is used to create a segment inside the memory.
+ `matrix = (int *)shmat(shmid, NULL, 0)` to attach the segment into the data space from the process
+ `shmdt(matrix)` to detached segment from data space from process.
+ `shmctl(shmid, IPC_RMID, NULL)` to find out or to edit the information about certain shared memory.

To show the result of the two matrix multiplication, we saved the result inside the `matrix[i][j]` and then printed according to the row & column order. 

```
 for (i = 0; i < row; i++){
        for (j = 0; j < col; j++){
            matrix[i*col + j] = C[i][j];
            printf("%d\t", matrix[i*col + j]);
        }
        printf("\n");   
    }
```

#### 2 b.  Create a program using the output matrix of the previous program (program soal2a.c) (Note!: Use shared memory). Then the matrix will be calculated with the new matrix. As follows an example of calculation for the existing matrix. The calculation is that each cell originating from matrix A becomes a number for factorial, then cells from matrix B become the maximum factorial limit (from largest to smallest) (Note!: Use threads for calculations in each cell).

##### Explanation 2b

We first made a matrix which will be used as <i>shared memory</i> and thread for each value of the matrix result on 2a which will also be run using faktorial <i>routine</i>.
  ```C
  int matriks[4][6];

  unsigned long long result[4][6];
  
  int matriksInput[4][6] = {
       {1, 2, 3, 4, 5, 6},
       {1, 1, 1, 1, 1, 1},
       {2, 2, 2, 2, 2, 2},
       {1, 0, 1, 0, 1, 0}
  };
  ```
  

Defining the three matrix which are `matriks` (4 × 6)  as the previouse problem matrix calculation, `matriksInput` (4 × 6) which is matrix used as the factorial calculation, and `result` (4 × 6) as the factorial matrix result.
 
  
  Next, definint the <i>struct</i>. 
  ```C
  struct args {
       int i;
       int j;
       int matriksA;
       int matriksB;
  };
  ```
 In here, `i` as the row, `j` as the column, `matriksA` as the matrix that will prepare the calculation from 2a, and `matriksB` which is matrix used as factorial calculation process 
 
  Making the factorial function.
  ```C
  unsigned long long faktorial(int n) {
       if (n == 0) {
            return 1;
       }

       else {
            return n * faktorial(n - 1);
       }
  }
  ```

Receive input from parameter **n**, if **n** is 0, the factorial will be 1 . Beside that, it will do an interative calculation until **n** reaches 0.
  
Below is code for the factorial function with condition according to the problem.
  
  ```C
  void *faktorial2(void *arg) {
       struct args *index = (struct args *) arg;
       int a = index -> matriksA;
       int b = index -> matriksB;

       int i = index -> i;
       int j = index -> j;

       if (a == 0 | b == 0) {
            result[i][j] = 0;
       }

       else if (a >= b) {
            result[i][j] = faktorial(a) / faktorial(a - b);
       }

       else if (b > a) {
            result[i][j] = faktorial(a);
       }
  }
  ```
  
  - `i` and `j` will be set-up with the row and column of the matrix. Then, `matriksA` and `matriksB` will be set into `a` and `b`.
  - If `a` and `b` bernilai 0, value of matriks `result[i][j]` will become 0.
  - If `a` bigger than equal to `b`, matrix `result[i][j]` will become the division result of `faktorial(a)` and `faktorial(a - b)`.
  - If `b` bigger than `a`, value of matrix `result[i][j]` will become `faktorial(a)`.
  
  Create a shared memory for `matriks` according to the template on modul 3.
  
  ```C
  int main() {
    key_t key = 1234;
    int (*value)[6];

    int shmid = shmget(key, 95, IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int *p = (int *) value;

    memcpy(matriks, p, 95);
  
  . . .
  
  ```
  We first create the key. Pointer  `p` will point to each of the array matrix value and there will also be copying value process using `memcpy` using pointer variable `p`to the matrix variable so that matrix will be filled of the matrix result that has been multiplied from the previous problem.
  
  The main function to solve the problem using thread.
  ```C
  pthread_t tid[4][6];

  printf("Matriks: \n");
  for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 6; j++) {
            printf("%4d", matriks[i][j]);
       }

       printf("\n");
  }

  for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 6; j++) {
            struct args *index = (struct args *) malloc(sizeof(struct args));
            index -> i = i;
            index -> j = j;
            index -> matriksA = matriks[i][j];
            index -> matriksB = matriksInput[i][j];

  pthread_create(&tid[i][j], NULL, &faktorial2, (void *)index);
       }
  }

  for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 6; j++) {
            pthread_join(tid[i][j], NULL);
       }
  }
  ```
  
  - Defining `tid` and `pthread` with total matrix ordo result and a struct which contain `index` attribute.
  - Loop the matrix which save the multiplication result from 2a to check wether the matrix already have same value with the previous problem (<i>debugging</i>).
  - First looping as the  row indication and second looping as the column indication.
  - <i>Thread</i> will be created with `pthread_create(&tid[i][j], NULL, &perkalian, (void *)index)` and will be run with `tid` `i` and `j` which will be incremented for everu loop.
  - Joining all created thread with `pthread_join(tid[i][j], NULL)`.

  Printing matrix result.
  ```C
  printf("Hasil Matriks: \n");
  
  for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 6; j++) {
            printf("%4llu ", result[i][j]);
       }
       printf("\n");
  }
  
  printf("\n");
  
  shmdt(value);
  shmctl(shmid, IPC_RMID, NULL);
  ```
  <i>Print</i> with `printf("%4llu ", result[i][j])` and <i>increment</i> for the amount of the row & column. `%4llu` here is the amount of <b>unsigned long long</b> character  which then printed from `result[i][j]`. To end the <i>shared memory</i>, we use `shmctl`.


#### 2 c.  For fear of lags in the process of helping Loba, Crypto also created a program (soal2c.c) to check the top 5 processes consuming computer resources with the command  “ps aux | sort -nrk 3,3 | head -5” (Note !: You must use IPC Pipes)


##### Explanation 2c

To create the program, we use 2 pipe and 2 fork. The creation of pope done with pipe() function. If the first fork **p_id1** and second fork **p_id2** successfully done and child process also successfully created (**p_id1 == 0 & p_id2 == 0**), The first pipe **pipe0[0]** will be duplicated into file descriptor from stdout, and then first pipe will be closed and program will execute command **ps aux**.

```
if (p_id1 == 0) 
	{ 
        	p_id2 = fork();
        	
        	if (p_id2 == 0) {
        	close(pipe0[0]);
        	close(pipe1[1]);
            	close(pipe1[0]);
        	
        	dup2(pipe0[1], STDOUT_FILENO);
    
	        close(pipe0[1]);
	        	
	        
        	execv("/usr/bin/ps", arg1);
		}  
		
	}

```

Then, parent process **p_id2** will be executed after closing the second pipe (**pipe1**), duplicating the first pipe **pipe0[0]** into file descriptor of stdin, duplicating second pipe **pipe1[1]** into file descriptor of stdout, and close the first pipe. Parent process will execute command **sort -nrk 3,3**.

```
	else {
			
			close(pipe0[1]);
		        close(pipe1[0]);
		        
		        dup2(pipe0[0], STDIN_FILENO);
		        dup2(pipe1[1], STDOUT_FILENO);
		        
		        close(pipe1[1]);
		        close(pipe0[0]);

		        execv("/usr/bin/sort", arg2);	
		    }
```

Finally, parent process **p_id1** will be executed after closing the second pipe (**pipe1**), duplicating the first pipe **pipe0[0]** into file descriptor of stdin, duplicating second pipe **pipe1[1]** into file descriptor of stdout, and close the first pipe. Parent process will execute command **head -5**.

```
else {
		
        	close(pipe0[1]);
        	close(pipe0[0]);
        			
        	dup2(pipe1[0], STDIN_FILENO);
      		//dup2(pipe1[1], STDOUT_FILENO);
        			
        	close(pipe1[0]);
	        close(pipe1[1]);
		
        	execv("/usr/bin/head", arg3); 
       }
     
```

##### Problem encountered
- Time constraint to solve the problem during ETS.

#### Result Image:
| ![](/Image/soal2/2a.png) |
|:--:| 
| *2a calculation result* |

| ![](/Image/soal2/2b.png) |
|:--:| 
| *2b calculation result* |

| ![](/Image/soal2/2c.png) |
|:--:| 
| *2c pipe IPC result* |


### NO 3
#### function to check the file type, whether the file is a Regular file or not:

	int isRegular(const char *path){ //cek file/dir
    struct stat path_stat;
    stat(path,&path_stat);
    return S_ISREG(path_stat.st_mode);
    }
    
For this function use a struct stat which contains several elements. To check whether the file is a regular type (not a folder), use one of the struct elements, namely st_mode.

#### moveFile function to categorize files
	char *ndir,*dir;
    char x='/'; // home/usr/modul /soal.c
    char y='.';
    dir = strrchr(file,x); //yg pertamakali dijumpai
    ndir = strchr(file, y); 
    char ext[1000];

strrchr is used to get / separated strings by checking from the back of the string. Meanwhile, strchr is used to get strings separated by signs. by checking from the front string.

	if (dir){
        //ngecek file/dir/ada ngga
        if(checkIfFileExists(file)){
            cekExt(dir+1,ext);
        }
        else{
            return 0;
        }
    }

If the path to be checked is a directory or a file, it will check whether the file path exists with the checkIfFileExists function. If there is, it will separate the extension from the file name with the CekExt function which will be explained in the 3d section.

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
    
Create a directory in the form of a category from the path inputted with mkdir. Then, the directory is stored in the current working directory by renaming it to the initial path of that directory.

#### to check the file exists
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
#### to check extension
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
    
- The default case (of looping when separating strings with the delimiter ".") Is categorizing files according to the extension where "." Occurs. only once (for example: the get.sh file will be categorized to a folder named sh).
- The first case is to categorize hidden files so that they can enter the hidden folder. If there is a "." in the first index (which indicates file-, fileExt (which will store the folder name) will store "hidden" so that a "hidden" folder will be created for hidden files.
- The second case is if there is no extension (count less than 1), so fileExt will save "unknown" so that an "unknown" folder will be created for files without extension.
- The third case is when the "." more than 1 (count is more than equal to 3 because it is counted as many tokens not "."). If the extension is more than 1, will enter the folder with the leading point. fileExt will store the value in step 2, and create a folder according to the string obtained in step 2.

#### For a number 3b, and 3c also use the same function that is to do a listing files recursively.
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


#### 3 a. Program accepts -f option like stated above, with this option the user may add file arguments to be categorized as much as they want. 

##### Explanation 3a

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
    
To see if it matches the requested input argument which is -f use strcmp to compare the input argument. Then, pthread_create and pthread_join are generated for each input path. If the file argument successfully categorized the output will Berhasil Dikategorikan, whereas if unsuccessful the output will Sad, gagal :(

#### 3 b. Program may accept -d option to categorize a directory instead. With this option, the user may only input 1 directory as it's arguments, unlike the -f option where the user may input multiple file arguments. The command above will categorize the files in /path/to/directory, the categorization result will be saved in the working directory where the C program is called (categorization result is not located at /path/to/directory).
	
##### Explanation 3b
	 else if(strcmp(argv[1],"-d")==0){
        if(argc==3){ //bisa menerima 1 path
            strcpy(dir, argv[2]);
            
        }
    }
- to Accept argument '-d'. Using strcmp to compare input arguments to whether they match the requested input argument which is -d. Command -d will be executed if you only enter 1 path, then use argc == 3. The path will be stored in a char dir.
 
		 if(!listFilesRecursively(dir)){
		printf("Yah, gagal disimpan :(\n");
		exit(0);
	    }
- The message that is displayed when unsuccessfully executing the command on the -d argument. 
	
		if(strcmp(argv[1],"-d")==0) printf("Berhasil disimpan\n");
		
- The message that is displayed when successfully executing the command on the -d argument. 
This message will be displayed when the recursive process to move all contents in the directory path that has been categorized into the current working directory has been successful.

#### 3 c. Other than the above options, this program accepts the * option. This option categorizes all the file in the working directory when the C program is run
	
##### Explanation 3c
	else if(strcmp(argv[1],"*")==0){
        strcpy(dir,src);
    }
- to Accept argument '*'. Using strcmp to compare input arguments to whether they match the requested input argument which is *. The path will be stored in a char dir.

#### 3 d. All files must be moved into a folder. Files without extensions will be moved to a folder named "Unknown". Hidden files will be moved to a folder named "Hidden".

##### Explanation 3d
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
    
Hidden files are files that have a prefix. in the file name, so that if the file begins with. it will create a Hidden file category. Then, if the file doesn't have any extensions later it will be categorized as Unknown. For extension names written in uppercase, it can be changed to lowercase using tolower.

#### 3 e. Each file to be categorized will be processed by a thread to make the program run in parallel to make it quicker.

##### Explanation 3e
	 for(;i<indeks;i++){
		printf("%s\n",save[i]);
		pthread_create(&tid2[i],NULL,moveFile,(void *)save[i]);
	    }
	    for(;j<indeks;j++)
	    {
		void *ptr;
		pthread_join(tid2[j],&ptr);
	    }
- Creating threads for the -d and * commands. pthread_create and pthread_join will be created as many categories as you want to create from files in that directory.
