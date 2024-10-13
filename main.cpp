#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <arm_neon.h>
#include <chrono>

typedef struct {
    int **A;
    int **B;
    int **C;
    int N; // matrix size
    int row; // row index
} thread_data;

// Initialize matrix with given size and sparsity
void initialize_matrix(int **matrix, int rows, int cols, double sparsity) 
{
    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            matrix[i][j] = (rand() < sparsity * RAND_MAX) ? rand() % 10 : 0;
        }
    }
}

// Matrix multiplication for a single row (threaded)
void *multiply(void *arg) {
    thread_data *data = (thread_data *)arg;  // Cast the void * to thread_data *
    int **A = data->A;
    int **B = data->B;
    int **C = data->C;
    int N = data->N;
    int i = data->row;  // The row this thread is responsible for

    for (int j = 0; j < N; j++) {
        C[i][j] = 0;
        for (int k = 0; k < N; k++) {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
    return NULL;
}

// Multi-threaded matrix multiplication
void multithread(int **A, int **B, int **C, int N, int thread_count) {
    pthread_t threads[thread_count];
    thread_data thread_args[thread_count];

    for (int i = 0; i < thread_count; i++) {
        thread_args[i].A = A;
        thread_args[i].B = B;
        thread_args[i].C = C;
        thread_args[i].N = N;
        thread_args[i].row = i;  // Assign each thread to handle a row
        pthread_create(&threads[i], NULL, multiply, (void *)&thread_args[i]); // Fixed parentheses here
    }
    
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

// SIMD-based matrix multiplication (using SSE)
void simd_neon(int **A, int **B, int **C, int N) {
      for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; j += 4) {  // Process 4 elements at a time
            int32x4_t c = vdupq_n_s32(0); // Initialize 4-element vector with zeros
            for (int k = 0; k < N; ++k) {
                int32x4_t a = vdupq_n_s32(A[i][k]); // Broadcast A[i][k] to all elements of the vector
                int32x4_t b = vld1q_s32(&B[k][j]);  // Load 4 elements from B[k][j]
                c = vmlaq_s32(c, a, b);             // Multiply and accumulate
            }
            vld1q_s32(&C[i][j]); // Store the result back to C[i][j]
        }
    }
}
// Cache-optimized (basic) matrix multiplication
void cache_optimized(int **A, int **B, int **C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

// Main function
int main(int argc, char *argv[]) {
    int N = 5000; // Example matrix size
    int num_threads = 4; // Example number of threads
    double sparsity = 1; // Example sparsity

    // Allocate and initialize matrices
    int **A = (int**) malloc(N * sizeof(int *));
    int **B = (int**) malloc(N * sizeof(int *));
    int **C = (int**) malloc(N * sizeof(int *));
    for (int i = 0; i < N; ++i) {
        A[i] = (int*) malloc(N * sizeof(int));
        B[i] = (int*) malloc(N * sizeof(int));
        C[i] = (int*) malloc(N * sizeof(int));
    }

    srand(time(NULL));  // Seed for random number generation
    initialize_matrix(A, N, N, sparsity);
    initialize_matrix(B, N, N, sparsity);

    // Choose optimization configuration
    int use_multithreading = 1;
    int use_simd = 1;
    int use_cache_optimization = 0;
    
    // Perform matrix multiplication based on configuration
    auto start = std::chrono::high_resolution_clock::now();

    if (use_multithreading) {
        printf("Running multithreaded version...\n");
        multithread(A, B, C, N, num_threads);
    } else if (use_simd) {
        printf("Running SIMD version...\n");
        simd_neon(A, B, C, N);
    } else if (use_cache_optimization) {
        printf("Running cache-optimized version...\n");
        cache_optimized(A, B, C, N);
    } else {
        printf("Running default single-threaded version...\n");
        for (int i = 0; i < N; ++i) {
            thread_data data = { A, B, C, N, i };
            multiply(&data);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    // Free allocated memory
    for (int i = 0; i < N; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
