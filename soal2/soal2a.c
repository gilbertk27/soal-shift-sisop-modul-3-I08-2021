#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/ipc.h>

pthread_t t_id[24];

int A[4][3] = { {1, 1, 1},
                {1, 2, 3},
                {1, 1, 1},
                {2, 2, 2} 
              },
    B[3][6] = { {1, 2, 3, 4,  5, 6},
                {1, 0, 1, 0, 1, 0},
                {1, 2, 4, 4, 1, 2} 
              },
    C[4][6];

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
   
    /*if (pthread_equal(id, t_id[0]))      // baris 1 matriks c
        for (int i = 0; i < 6; i++) 
            C[0][i] = A[0][0]*B[0][i] + A[0][1]*B[1][i] + A[0][2]*B[2][i];
    else if (pthread_equal(id, t_id[1])) // baris 2 matriks c
        for (int i = 0; i < 6; i++)
            C[1][i] = A[1][0]*B[0][i] + A[1][1]*B[1][i] + A[1][2]*B[2][i];
    else if (pthread_equal(id, t_id[2])) // baris 3 matriks c
        for (int i = 0; i < 6; i++)
            C[2][i] = A[2][0]*B[0][i] + A[2][1]*B[1][i] + A[2][2]*B[2][i];
    else if (pthread_equal(id, t_id[3])) // baris 4 matriks c
        for (int i = 0; i < 6; i++)
            C[3][i] = A[3][0]*B[0][i] + A[3][1]*B[1][i] + A[3][2]*B[2][i];
	*/
   }
}

int main () {

    int i, j, err;
    for (i = 0; i < 24; i++) {
        err = pthread_create(&(t_id[i]), NULL, &startcount, NULL);
    }

    for (i = 0; i < 24; i++) {
	    pthread_join(t_id[i], NULL);
    }

/*
    pthread_join(t_id[0], NULL);
    pthread_join(t_id[1], NULL);
    pthread_join(t_id[2], NULL);
    pthread_join(t_id[3], NULL);
*/
    printf("MATRIX RESULT\n");

    int row = 4;
    int col = 6;
    int *matrix;

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

    return 0;
}
