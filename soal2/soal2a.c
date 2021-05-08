#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/ipc.h>

pthread_t tid[5];

// Ubah-ubah matriks disini!!!
int A[4][3] = { {1, 2, 3},
                {3, 4, 5},
                {5, 6, 7},
                {7, 8, 9} },
    B[3][6] = { {1, 3, 5, 7,  9, 11},
                {2, 4, 6, 8, 10, 12},
                {13, 14, 15, 16, 17, 18} },
    C[4][6];

void* playandcount (void *arg) {

    pthread_t id = pthread_self();

    if (pthread_equal(id, tid[0]))      // baris 1 matriks c
        for (int i = 0; i < 6; i++) 
            C[0][i] = A[0][0]*B[0][i] + A[0][1]*B[1][i] + A[0][2]*B[2][i];
    else if (pthread_equal(id, tid[1])) // baris 2 matriks c
        for (int i = 0; i < 6; i++)
            C[1][i] = A[1][0]*B[0][i] + A[1][1]*B[1][i] + A[1][2]*B[2][i];
    else if (pthread_equal(id, tid[2])) // baris 3 matriks c
        for (int i = 0; i < 6; i++)
            C[2][i] = A[2][0]*B[0][i] + A[2][1]*B[1][i] + A[2][2]*B[2][i];
    else if (pthread_equal(id, tid[3])) // baris 4 matriks c
        for (int i = 0; i < 6; i++)
            C[3][i] = A[3][0]*B[0][i] + A[3][1]*B[1][i] + A[3][2]*B[2][i];

}

int main () {

    for (int i = 0; i <5; i++) {
        int err = pthread_create(&(tid[i]), NULL, &playandcount, NULL);

        // if (err != 0)
        //     printf("\nCan't create thread: [%s]", strerror(err));
    
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);

    printf("MATRIKS PERKALIAN\n");

    int row = 4;
    int column = 6;
    int *matrix;

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int)*row*column, IPC_CREAT | 0666);
    matrix = (int *)shmat(shmid, NULL, 0);

    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            matrix[i*column + j] = C[i][j];
            printf("%d\t", matrix[i*column + j]);
        }
        printf("\n");   
    }

    sleep(10);

    shmdt(matrix);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
