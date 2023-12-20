#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

struct matrix {
    int rows;
    int cols;
    int **arr;
};

struct multiplication {
    int row;
    int col;
    struct matrix *A;
    struct matrix *B;
    struct matrix *C;
};

// Read matrix from file
int read_matrix(FILE *file, struct matrix *m) {
    int rows, cols;
    fscanf(file, "%d %d", &rows, &cols);
    m->rows = rows;
    m->cols = cols;
    int **arr = (int **) malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        arr[i] = (int *) malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            if (fscanf(file, "%d", &arr[i][j]) != 1) {
                printf("Error reading the file\n");
                return -1; } } }
    m->arr = arr;
    return 0;
}

// Write matrix to file and print to screen
void write_matrix(FILE *file, struct matrix *C) {
    for (int i = 0; i < C->rows; i++) {
        for (int j = 0; j < C->cols; j++) {
            fprintf(file, "%d ", C->arr[i][j]);
            printf("%d ", C->arr[i][j]);
        }
        fprintf(file, "\n");
        printf("\n");
    }
}

// Thread function for element-wise multiplication
void *thread_per_element(void *args) {
    struct multiplication *per_element;
    per_element = (struct multiplication *) args;
    int sum = 0;
    for (int k = 0; k < per_element->B->rows; k++) {
        sum += per_element->A->arr[per_element->row][k] * per_element->B->arr[k][per_element->col];
    }
    per_element->C->arr[per_element->row][per_element->col] = sum;
    free(per_element);
    pthread_exit(NULL);
}

// Thread function for row-wise multiplication
void *thread_per_row(void *args) {
    struct multiplication *per_row;
    per_row = (struct multiplication *) args;
    for (int j = 0; j < per_row->B->cols; j++) {
        int sum = 0;
        for (int k = 0; k < per_row->B->rows; k++) {
            sum += per_row->A->arr[per_row->row][k] * per_row->B->arr[k][j];
        }
        per_row->C->arr[per_row->row][j] = sum;
    }
    free(per_row);
    pthread_exit(NULL);
}

// Main function for element-wise multiplication
void thread_per_element_main(struct matrix *A, struct matrix *B, struct matrix *C) {
    int rows = A->rows, cols = B->cols;
    pthread_t threads[rows][cols];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            struct multiplication *per_element = malloc(sizeof(struct multiplication));
            per_element->row = i; per_element->col = j;
            per_element->A = A; per_element->B = B; per_element->C = C;
            if (pthread_create(&threads[i][j], NULL, thread_per_element, (void *) per_element)) {
                printf("Can not create a thread\n");
                exit(1);
            }
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }
}

// Main function for row-wise multiplication
void thread_per_row_main(struct matrix *A, struct matrix *B, struct matrix *C) {
    int n = A->rows;
    pthread_t threads[n];
    for (int i = 0; i < n; i++) {
        struct multiplication *per_row = malloc(sizeof(struct multiplication));
        per_row->row = i; per_row->col = 0;
        per_row->A = A; per_row->B = B; per_row->C = C;
        if (pthread_create(&threads[i], NULL, thread_per_row, (void *) per_row)) {
            printf("Can not create a thread\n");
            exit(1);
        }
    }
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error in arguments\n");
        return 0;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", argv[1]);
        return -1;
    }
    struct matrix *A = malloc(sizeof(struct matrix));
    struct matrix *B = malloc(sizeof(struct matrix));
    if (read_matrix(file, A) == -1)
        return 0;
    if (read_matrix(file, B) == -1)
        return 0;
    fclose(file);
    if (A->cols != B->rows) {
    FILE *file = fopen(argv[2], "w");
    if (file == NULL) {
        printf("Error opening the file: %s\n", argv[2]);
        return -1;
    }
    fprintf(file, "Multipication of Matrix is not Possible !!\n");
    fclose(file);
    printf("Columns of first matrix must be equal to rows of second matrix\n");
    return 0;
}

    struct matrix *C1 = malloc(sizeof(struct matrix));
    C1->rows = A->rows; C1->cols = B->cols;
    C1->arr = (int **) calloc(A->rows, sizeof(int *));
    for (int i = 0; i < A->rows; i++) {
        C1->arr[i] = (int *) calloc(B->cols, sizeof(int));
    }
    struct timeval start, end;
    gettimeofday(&start, NULL);
    thread_per_row_main(A, B, C1);
    gettimeofday(&end, NULL);
   double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    file = fopen(argv[2], "w");
    fprintf(file, "By Row\n");
    write_matrix(file, C1);
    printf("By Row\n");
    write_matrix(stdout, C1);
    fprintf(file, "Time : %f s\n", time_taken);
    printf("Time : %f s\n", time_taken);
    struct matrix *C2 = malloc(sizeof(struct matrix));
    C2->rows = A->rows; C2->cols = B->cols;
    C2->arr = (int **) calloc(A->rows, sizeof(int *));
    for (int i = 0; i < A->rows; i++) {
        C2->arr[i] = (int *) calloc(B->cols, sizeof(int));
    }
    gettimeofday(&start, NULL);
    thread_per_element_main(A, B, C2);
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    fprintf(file, "\nBy Element\n");
    write_matrix(file, C2);
    printf("\nBy Element\n");
    write_matrix(stdout, C2);
    fprintf(file, "Time : %f s\n", time_taken);
    printf("Time : %f s\n", time_taken);
    fclose(file);
    return 0;
}

