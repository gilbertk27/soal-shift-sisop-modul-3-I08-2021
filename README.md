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

Disini, untuk membuat program tersebut, kami menggunakan 2 pipe with 2 fork. Pembuatan pipe dapat dilakukan with fungsi **pipe()**. Apabila fork pertama **p_id1** and fork kedua **p_id2** berhasil dilakukan and child process berhasil dibuat **p_id1 == 0 & p_id2 == 0**, maka pipe pertama fd1[1]akan diduplikasi menjadi file deskriptor dari stdout, lalu pipe pertama tersebut fd1 akan ditutup and program akan mengeksekusi command **ps aux**.

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
