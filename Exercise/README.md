# Final Exercise Parallel Programming with MPI

### The exercise must me sent by email to matteo.barborini@uni.lu and georgios.kafanas@uni.lu before the 13th of January 2026.

__For any questions regarding the exercises please write to us!__

To submit the exercise, you need to create an archive of the source part (`src`) of the repository and send it via email. Assume that you work on the `main` branch and all your code is committed. 

Then, create the `TAR.GZ` archive with the following command:

```
git archive --format=tar.gz --output=${HOME}/MPI_exercise-<name>-<surname>.tar.gz --prefix=excercise/ main src/
```

Then, send the file `MPI_exercise-<name>-<surname>.tar.gz` to the emails specified above.

_Note:_ if you are working on a branch different that `main` or if you created a tag, then you can replace `main` with any valid commit or commit reference.

The exercise is divided into 5 parts (Please read carefully below):
- Exercise 1.1 (10 points)
- Exercise 1.2 (10 points)
- Exercise 1.3 (10 points)
- Exercise 2.1 (55 points)
- Exercise 2.2 (15 points)

for a total number of 100 points.

Please run the codes on one node of the AION cluster of the university of Luxembourg.


## Matrix computation in a single sequential process

Before proceeding to Parallelize matrix multiplication algorithms we review the single process (and single thread) implementations. Linear algebra operations are constrained by data access, and this is equally true to single process and multi-process computations. The overview presented here follows loosely the material of <a href=#1>[1]</a>.

Matrix computations are built on a hierarchy of linear algebra operations. Each level is using the memory access pattern of the previous level, even though each level may use bespoke procedures that do not call the procedures of the previous level. This architectural pattern is outlined in the design of BLAS <a href=#2>[2]</a>,<a href=#3>[3]</a>,<a href=#4>[4]</a>, an application interface for libraries of matrix computations, and you can investigate a concrete implementation in the reference implementation of BLAS in Netlib <a href=#5>[5]</a>.

### Vector scaling and dot-product

The first operation we implement is vector scaling. The corresponding function in the interface of BLAS is `xAXPY` and performs the operation:

$$
  y \gets \alpha x + y
$$

The memory access patter is represented by a for loop in pseudocode.

```
process xAXPY(n, alpha, x, incx, y, incy)
  for i = 0:(n-1)
    y[i*incy] <- alpha*x[i*incx] + y[i*incy]
  end
end
```

A very similar operation in terms of memory access is the dot-product. The corresponding function in the interface of BLAS is `xDOT` and performs the operation:

$$
  c \gets {x}^{T} y
$$

The memory access pattern is very similar to the vector scaling. The pseudocode for the `xDOT` function is the following.

```
function xDOT(n, x, incx, y, incy)
  c <- 0
  for i = 0:(n-1)
    c <- c + x[i*incx]*y[i*incy]
  end
  return c
end
```

This `xDOT` function like `xAXPY` reads 2 vectors, $x$ and $y$, sequentially. One way that data access in `xAXPY` differs from `xDOT` is that in `xAXPY` one of the input vectors is also written sequentially.

### **Exercise 1.1:** Implement the `DAXPY` and `DDOT` operations in C programs. (10 points)

---

### Matrix-vector multiplication

The matrix-vector multiplication operation in BLAS is performing the following operation.

$$
  y \gets \alpha A x + \beta y
$$

BLAS incorporates a scaling operations in the multiplication as they appear commonly in many applications. The operation performed by BLAS is summarized on the following pseudocode.

```
process xGEMV(m, n, alpha, A, ldA, x, incx, beta, y, incy)
  for j = 0:(m-1)
    for i = 0:(n-1)
      y[i*incy] <- alpha*A[i+j*ldA]*x[i*incx] + beta*y[i*incy]
    end
  end
end
```

The order of the index loops (`i` and `j`) determines the memory access pattern. Assume that $i \to j$ denotes a pair of nested loops where the outer loop loops over $i$. Then `xGEMV` follows the $j \to i$ order. The inner loop in fact performs the `xAXPY` operation over the columns of a matrix `A` that is stored in a column major order. In column major order an array $A$ is serialized as follows in the system memory.

$$
  A = \begin{pmatrix}
    A(1,1) & A(1,2) & \cdots & A(1,n) \\
    A(2,1) & A(2,2) & \cdots & A(2,n) \\
    \vdots & \vdots & \ddots & \vdots \\
    A(m,1) & A(m,2) & \cdots & A(m,n)
  \end{pmatrix}
  \to
  \mathtt{A} = \begin{bmatrix} A(1,1), A(2,1), \ldots, A(m,1), A(1,2), A(2,2), \ldots, A(m,2), \ldots, A(1,n), A(2,n), \ldots, A(m,n) \end{bmatrix}
$$

The pattern of memory access now matters. Due to caching, it is faster to access random access memory in a sequential manner. Thus the $j \to i$ loop
```
for j = 0:(m-1)
  for i = 0:(n-1)
    y[i*incy] <- alpha*A[i+j*ldA]*x[i*incx] + beta*y[i*incy]
  end
end
```
is faster than the $i \to j$ for loop
```
for i = 0:(n-1)
  for j = 0:(m-1)
    y[i*incy] <- alpha*A[i+j*ldA]*x[i*incx] + beta*y[i*incy]
  end
end
```
even though the perform the exact same number and type of operations and produce the same result. Note that the memory access pattern of the $i \to j$ inner loop is the same as the `xDOT` pattern, as the inner loop performs a dot product between the rows of $A$ and $y$.

### **Exercise 1.2:** Implement `DGEMV` and the same operation with data access pattern of `DDOT` (call it `rowwise_DGEMV`) in C programs. Run a few examples with both functions, and provide the run times. (10 points)

---

### Matrix-matrix multiplication

The matrix-matrix multiplication operands in BLAS is performing the following operation.

$$
  C \gets \alpha A B + \beta C 
$$

Again there are scaling operations incorporated as they often appear in many applications. The operation implementation in BLAS is summurized in the follwoing pseudocode.

```
process xGEMM(m, n, k, alpha, A, ldA, B, ldB, beta, C, ldC)
  for j = 0:(n-1)
    for l = 0:(k-1)
      for i = 0:(m-1)
        C[i+j*ldC] <- A[i+l*ldA]*B[l+j*ldB] + beta*C[i+j*ldC]
      end
    end
  end
end
```

As with the matrix-vector mutliplication we can see that the operation performed in the inner loop has the same data access patern to `xAXPY` to ensure that the most frequent access to the memory is sequential.

### **Exercise 1.3:** Implement the `DGEMM` of BLAS and the `DGEMM` that accesses $A$ in a row major manner ($j \to i \to l$) calling it `rowwise_DGEMM` in C programs. Run a few examples and provide the runtimes. For simplicity use square matrices with dimensions that are multiples of the number of cores on one AION node (128 cores). (10 points)

---

### Matrix-matrix multiplication parallelization in the message passing framework

Multiple algorithms for the parallelization of matrix-matrix multiplication over message passing frameworks have been developed, with each algorithm targeting a specific architecture family or optimizing for a specific performance aspect. Typical examples of such algorithms are Cannon's <a href=#6>[6]</a>, Fox's <a href=#7>[7]</a>, and SUMMA <a href=#8>[8]</a> algorithms. In this exercise we investigate message passing parallelizations of matrix-matrix multiplication, and we implement a simplified version of Cannon's algorithm.

In the following examples the multiplication 
$$
  C = A B
$$
where $A \in \mathbb{R}^{m \times k}$ and $B \in \mathbb{R}^{k \times n}$ is parallelized.

### Static scattering of a single operand

The simplest way to distribute the computation of the matrix-matrix product over $q$ processes is to spread one of the operands across all processes. This is based on the following tautology.

```math
  A B = A \begin{pmatrix} B_{[1:\lceil k/p \rceil]:*} & B_{[\lceil k/p \rceil+1:2\lceil k/p \rceil]:*} & \cdots & B_{[k - (p-1) \lceil k/p \rceil:k]:*}\end{pmatrix} = \begin{pmatrix} A B_{[1:\lceil k/p \rceil]:*} & A B_{[\lceil k/p \rceil+1:2\lceil k/p \rceil]:*} & \cdots & A B_{[k - (p-1) \lceil k/p \rceil:k]:*} \end{pmatrix}
```

In this notation $B_{[a:b]:*}$ are a submatrix of $B$ containing the columns from $a$ to $b$ included.

- The matrix $B$ is distributed once at the beginning of the computation, and the resulting fragments of $C$ are collected at the end.
- Each process must have sufficient memory to store all of $A$.

### **Exercise 2.1:** Implement a parallelized version of the matrix-matrix multiplication in C using static scattering of a single operand. For simplicity use square matrices with dimensions that are multiples of the number of cores on one AION node (128 cores). (55 points)

---

### Dynamic distribution of operands

The storage requirement can be reduced by distributing the matrix $A$ in row-wise bands across all processes. This is based on the following tautology.

```math
A B = \begin{pmatrix} A_{*:[1:\lceil k/p \rceil]} \\ A_{*:[\lceil k/p \rceil+1:2\lceil k/p \rceil]} \\ \vdots \\ A_{*:[k - (p-1) \lceil k/p \rceil:k]} \end{pmatrix} \begin{pmatrix} B_{[1:\lceil k/p \rceil]:*} & B_{[\lceil k/p \rceil+1:2\lceil k/p \rceil]:*} & \cdots & B_{[k - (p-1) \lceil k/p \rceil:k]:*} \end{pmatrix}
= \begin{pmatrix} A_{*:[1:\lceil k/p \rceil]} B_{[1:\lceil k/p \rceil]:*} & A_{*:[1:\lceil k/p \rceil]} B_{[\lceil k/p \rceil+1:2\lceil k/p \rceil]:*} & \cdots & A_{*:[1:\lceil k/p \rceil]} B_{[k - (p-1) \lceil k/p \rceil:k]:*} \\
A_{*:[\lceil k/p \rceil+1:2\lceil k/p \rceil]} B_{[1:\lceil k/p \rceil]:*} & A_{*:[\lceil k/p \rceil+1:2\lceil k/p \rceil]} B_{[\lceil k/p \rceil+1:2\lceil k/p \rceil]:*} & \cdots & A_{*:[\lceil k/p \rceil+1:2\lceil k/p \rceil]} B_{[k - (p-1) \lceil k/p \rceil:k]:*} \\
\vdots & \vdots & & \vdots \\ A_{*:[k - (p-1) \lceil k/p \rceil:k]} B_{[1:\lceil k/p \rceil]:*} & A_{*:[k - (p-1) \lceil k/p \rceil:k]} B_{[\lceil k/p \rceil+1:2\lceil k/p \rceil]:*} & \cdots & A_{*:[k - (p-1) \lceil k/p \rceil:k]} B_{[k - (p-1) \lceil k/p \rceil:k]:*} \end{pmatrix}
```

The pseudocode implementing the operation is the following.

```
process row_wise_cannon(A, B, C)
  id <- rank()
  p <- size()
  recieve(0, A(*,id))
  recieve(0, B(id,*))
  for i = 0:(p-1)
    C(p,id) <- A(*,(i+id) mod p)*B(id,*)
    if i < p-1
      send((id+1) mod p, A(*,(i+id) mod p))
      recieve((id-1) mod p, A(*,((i+1)+id) mod p))
    end
  end
  send(0,C(*,id))
end
```

### **Exercise 2.2:** Implement a parallelized version of the matrix-matrix multiplication in C using dynamic scattering of a single operand. For simplicity use square matrices with dimensions that are multiples of the number of cores on one AION node (128 cores). (15 points)

---

#### References

1. <a id="1"></a> Golub, Gene H. 1965. "Numerical Methods for Solving Linear Least Squares Problems." *Numerische Mathematik* 7 (3): 206–16. DOI: [10.1007/bf01436075](https://doi.org/10.1007/bf01436075).
2. <a id="2"></a> Dongarra, J. J., Jeremy Du Croz, Sven Hammarling, and I. S. Duff. 1990. "A Set of Level 3 Basic Linear Algebra Subprograms." *ACM Trans. Math. Softw.* (New York, NY, USA) 16 (1): 1–17. DOI: [10.1145/77626.79170](https://doi.org/10.1145/77626.79170).
3. <a id="3"></a> Dongarra, Jack J., Jeremy Du Croz, Sven Hammarling, and Richard J. Hanson. 1988. "An Extended Set of FORTRAN Basic Linear Algebra Subprograms." *ACM Trans. Math. Softw.* (New York, NY, USA) 14 (1): 1–17. DOI: [10.1145/42288.42291](https://doi.org/10.1145/42288.42291).
4. <a id="4"></a> Lawson, C. L., R. J. Hanson, D. R. Kincaid, and F. T. Krogh. 1979. "Basic Linear Algebra Subprograms for Fortran Usage." *ACM Trans. Math. Softw.* (New York, NY, USA) 5 (3): 308–23. DOI: [10.1145/355841.355847](https://doi.org/10.1145/355841.355847).
5. <a id="5"></a> "BLAS (Basic Linear Algebra Subprograms)." https://netlib.org/blas/
6. <a id="6"></a> Cannon, Lynn Elliot. 1969. "A Cellular Computer to Implement the Kalman Filter Algorithm." PhD thesis, Montana State University.
7. <a id="7"></a> Fox, G. C., S. W. Otto, and A. J. G. Hey. 1987. "Matrix Algorithms on a Hypercube I: Matrix Multiplication." *Parallel Computing* 4 (1): 17–31. DOI: [10.1016/0167-8191(87)90060-3](https://doi.org/10.1016/0167-8191(87)90060-3).
8. <a id="8"></a> Van De Geijn, R. A., and J. Watts. 1997. "SUMMA: Scalable Universal Matrix Multiplication Algorithm." *Concurrency: Practice and Experience* 9 (4): 255–74. DOI: [10.1002/(sici)1096-9128(199704)9:4&lt;255::aid-cpe250&gt;3.0.co;2-2](https://doi.org/10.1002/(sici)1096-9128(199704)9:4<255::aid-cpe250>3.0.co;2-2).
