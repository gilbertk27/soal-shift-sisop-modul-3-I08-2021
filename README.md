# soal-shift-sisop-modul-3-I08-2021
 
 
### NO. 2
#### 2 a.  Create a matrix multiplication program (4x3 and 3x6) and then display the results. The matrix will contain the numbers 1-20 (no need to create a number filter).

##### Explanation 2a

- The multiplication between 4x3 and 3x6 matrix on 2a done so that it will result in 4x6 matrix. To do the calculation, we make a startcount thread which is decalred as follows.
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

- Because the result of the matrix multiplication will be used on 2b, after done with the calculation, the result then saved in shared memory with code as follows.

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

- Here's a brief explanation of the code above:
+ `shmid` will return the value after we call the system on `shmget(key, sizeof(int)*row*column, IPC_CREAT | 0666)` which is used to create a segment inside the memory.
+ `matrix = (int *)shmat(shmid, NULL, 0)` to attach the segment into the data space from the process
+ `shmdt(matrix)` to detached segment from data space from process.
+ `shmctl(shmid, IPC_RMID, NULL)` to find out or to edit the information about certain shared memory.

- To show the result of the two matrix multiplication, we saved the result inside the `matrix[i][j]` and then printed according to the row & column order. 

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

#### 2 c.  For fear of lags in the process of helping Loba, Crypto also created a program (soal2c.c) to check the top 5 processes consuming computer resources with the command  “ps aux | sort -nrk 3,3 | head -5” (Note !: You must use IPC Pipes)


##### Explanation 2c


##### Problem encountered
- lack of knowledge to do the problem so the time ran out to read refferences.
- don't know the appropriate function & library to solve the problem
- placement of the variable that won't be read based on the placement (inside child process or not)

