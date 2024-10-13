# Advanced-Computer-Systems-Project-2

Installs for this project:
clang compiler (brew install gcc47)

Notes:
- SIMD is not compatible with Mac M1s - used ARM NEON (Documentation: https://developer.arm.com/documentation/den0018/latest/)
- I was unable to get a matrix of size 10,000 x 10,000 to run on my computer with neon or cache optimization, so I resized the matrix to 5000 x 5000

PseudoCode:

main()
{
int N
int num_threads
double sparsity

intitalize matrix A and B with size NxN and sparsity 'sparsity' with random values

Decide if we are using multithreading, Neon, or Cache Optimization
  call corresponding funtion
}

// multithreading mult
multiply()
{
 multiply a row of matrix A with all columns of matrix B.
}

Multithread()
{
manage the rows of matrix A
each thread is one row of matrix A
}

// mult using NEON
simd_neon()
{
creates sub-vectors of matrix A and B that are each 4 elements
multiplies them together and accumulates them 
stores results in matrix C
}

// mult using cache optimization
cache_optimized()
{
standard matrix muliplication
  starts at A[0][0], mulitplies it with B[0][0] 
  adds result to A[0][1]*B[1][0]
  continue until you reach A[0][N]*B[N][0]
repeat process for A[1][0], B[0][1]
until you reach A[N][0], B[0][N]
}

