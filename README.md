# Multithreaded Matrix Multiplication

This program performs matrix multiplication using multithreading. It creates separate threads for each row or each element of the resulting matrix, depending on the chosen mode.

## How it works

The program uses the POSIX threads (pthreads) library to create threads. There are two modes of operation:

1. **Per-row mode**: In this mode, a separate thread is created for the computation of each row of the resulting matrix. Each thread calculates an entire row of the result by performing the dot product of a row from the first matrix and each column from the second matrix.

2. **Per-element mode**: In this mode, a separate thread is created for the computation of each individual element of the resulting matrix. Each thread calculates one element of the result by performing the dot product of a row from the first matrix and a column from the second matrix.

## Compilation

To compile the program, use the following command:

```bash
gcc -o matmul matmul.c -lpthread
```

## Input & Output

The input file should follow this format:

[number of rows matrix1] [number of columns matrix1]

element11 element12 ... element1N

[number of rows matrix2] [number of columns matrix2]

element11 element12 ... element1N

The Output should be like this:

[result matrix entries]

END1 [elapsed time of procedure 1]

[result matrix entries]

END2 [elapsed time of procedure 2]

For example: if the input file is like this

3 5

6 1 3 5 2

9 2 1 6 1

9 0 5 6 3

5 4

7 7 4 6

3 5 4 1

0 2 5 6

8 4 4 6

1 3 8 1

The Output should be like this:

By Row

87 79 79 87 

118 102 81 99 

114 106 109 123 

Time : 0.002181 s



By Element

87 79 79 87 

118 102 81 99 

114 106 109 123 

Time : 0.003993 s
