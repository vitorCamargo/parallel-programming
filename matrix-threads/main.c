#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

void *mult(void* arg) {
    int *data = (int *)arg;
    int k = 0, i = 0;

    int x = data[0];
    for (i = 1; i <= x; i++) {
        k += data[i]*data[i+x];
    }

    int *p = (int*)malloc(sizeof(int));
    *p = k;

    pthread_exit(p);
}

int **aloca(int size) {
    int *array = (int*) malloc(size * size * sizeof(int));
    int **matrix = (int**) malloc(size * sizeof(int*));
    int i;

    for (i = 0; i < size; i++) {
        matrix[i] = &array[i * size];
    }

    return matrix;
}

void randomize(int **matrix, int max) {
    int i, j;

    for (i = 0; i < max; i++) {
        for (j = 0; j < max; j++) {
            // matrix[i][j] = rand()%10;
            matrix[i][j] = 1;
        }
    }
}

void printa(int **matrix, int max) {
    int i, j;

    for (i = 0; i < max; i++) {
        for(j = 0; j < max; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <tamanho_matrix>\n", argv[0]);
        exit(0);
    }

    int MAX = atoi(argv[1]);

    int **matrixA = aloca(MAX);
    int **matrixB = aloca(MAX);

    randomize(matrixA, MAX);
    randomize(matrixB, MAX);

    printf("MATRIX A: \n");
    printa(matrixA, MAX);

    printf("\n\nMATRIX B: \n");
    printa(matrixA, MAX);

    printf("\n-------------\n");
      
    int r1 = MAX, c1 = MAX, r2 = MAX, c2 = MAX, i, j, k;
    int max_value = r1*c2;

    pthread_t *threads;
    threads = (pthread_t*)malloc(max_value*sizeof(pthread_t));

    int count = 0;
    int* data = NULL;
    for (i = 0; i < r1; i++) {
        for (j = 0; j < c2; j++) {
            data = (int *)malloc((20)*sizeof(int));
            data[0] = c1;

            for (k = 0; k < c1; k++) {
                data[k+1] = matrixA[i][k];
            } 

            for (k = 0; k < r2; k++) {
                data[k+c1+1] = matrixB[k][j];
            }

            pthread_create(&threads[count++], NULL,  mult, (void*)(data));
        }
    }

    printf("RESULTADO: \n");
    for (i = 0; i < max_value; i++) {
        void *k;
        pthread_join(threads[i], &k);

        int *p = (int *)k;
        printf("%d ",*p); 
        if ((i + 1) % c2 == 0) {
            printf("\n"); 
        }
    }
}