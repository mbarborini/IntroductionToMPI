# MPI Derived Datatypes Examples 

## 1.1 `MPI_contiguous`

* Assume 4 processes

* A 4x4 array is created on rank 0

* A contiguous datatype is created using `MPI_Type_contiguous`

* Rows are distributed to all process

## 2.1 `MPI_Type_vector`

* Create vector datatype: 4 blocks, 1 element, each element presents a stride of 4 with respect to the previous one 

* Create 4x4 array on rank 0

* Using the new vector datatype, we send the columns to different processes

## 2.2 `MPI_Type_vector`

* Same as the previous example, but now we send matrix tiles 

* We define a vector datatype

* For simplicity the data is send into vectors of length 0 (plain submatrices)

## 2.3 `MPI_Type_vector`

* This exercise only works for two processes

* We define a vector datatype that selects certain elements of a 1D vector

* The selected elements belong to a group of three. Each group is three elements apart from the previous group. 

## 3.1 `MPI_Type_create_subarray`

* This is a complex exercise

* We send matrix tiles using `MPI_Scatterv`

* For that purpose, we need to create a new type and use `MPI_Type_create_subarray`

* In order scatter the tiles,  we need to extend the subarray datatype using `MPI_Type_create_resized`.

* This last transformation allow us to simplify the scattering by providing a displacement array.

* It assumes 4 processes.

## 4.1 `MPI_Type_indexed`

* This exercise only works for 2 processes

* The idea is to use `MPI_Type_indexed` to send the lower triangular elements of a matrix from one process to another

* The most interesting part corresponds to the calculation of the displacements. 

## 5.1 `MPI_Pack`

* We use `MPI_Bcast` to send a double-precision and an integer value from rank 0 to the other ranks

* In order to send two different types, we use `MPI_Pack` and pack the data into the buffer `tambuff`

* Once received the other ranks will unpack the data using `MPI_Unpack`

* We can select as many processes as we want