#include <stdio.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int matriks[4][6];
unsigned long long result[4][6];

int matriksInput[4][6] = {
    {1, 2, 3, 4, 5, 6},
    {1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2},
    {1, 0, 1, 0, 1, 0}
};

struct args {
    int i;
    int j;
    int matriksA;
    int matriksB;
};

unsigned long long faktorial(int n) {
    if (n == 0) {
        return 1;
    }

    else {
        return n * faktorial(n - 1);
    }
}

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
        int temp = a - b;
        result[i][j] = faktorial(a) / faktorial(temp);
    }

    else if (b > a) {
        result[i][j] = faktorial(a);
    }
}

int main() {
    key_t key = 1234;
    int (*value)[6];

    int shmid = shmget(key, 95, IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int *p = (int *) value;

    memcpy(matriks, p, 95);

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
            
            pthread_create(&tid[i][j], NULL, &faktorial2, (void *) index);
        }
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            pthread_join(tid[i][j], NULL);
        }
    }
    
    printf("\nHasil Matriks: \n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            printf("%4llu ", result[i][j]);
        }

        printf("\n");
    }

    printf("\n");

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
