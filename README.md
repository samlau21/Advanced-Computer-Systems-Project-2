# Advanced-Computer-Systems-Project-2

Installs for this project:<br />
- clang compiler (brew install gcc47)

Notes:
- SIMD is not compatible with Mac M1s - used ARM NEON (Documentation: https://developer.arm.com/documentation/den0018/latest/)
- I was unable to get a matrix of size 10,000 x 10,000 to run on my computer with neon or cache optimization, so I resized the matrix to 5000 x 5000

How to use:
- Download main.cpp
- to change thread number, change variable 'num_threads' in main
- to change matrix size, change variable 'N' in main
- to change sparsity, change 'sparsity' in main
<br />
<br />
PseudoCode:

main()
{<br />
int N <br />
int num_threads<br />
double sparsity<br />

intitalize matrix A and B with size NxN and sparsity 'sparsity' with random values<br />
<br />
Decide if we are using multithreading, Neon, or Cache Optimization<br />
  call corresponding funtion<br />
}<br />
<br />
// multithreading mult<br />
multiply()<br />
{<br />
 multiply a row of matrix A with all columns of matrix B.<br />
}
<br /><br />
Multithread()<br />
{<br />
manage the rows of matrix A<br />
each thread is one row of matrix A<br />
}
<br /><br />
// mult using NEON
simd_neon()<br />
{<br />
creates sub-vectors of matrix A and B that are each 4 elements<br />
multiplies them together and accumulates them <br />
stores results in matrix C <br />
}
<br /><br />
// mult using cache optimization<br />
cache_optimized()<br />
{<br />
standard matrix muliplication<br />
  starts at A[0][0], mulitplies it with B[0][0] <br />
  adds result to A[0][1]*B[1][0] <br />
  continue until you reach A[0][N]*B[N][0] <br />
repeat process for A[1][0], B[0][1] <br />
until you reach A[N][0], B[0][N] <br />
}

