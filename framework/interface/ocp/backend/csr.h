#ifndef _OCP_INTERFACE_BACKEND_CSR_H_
#define _OCP_INTERFACE_BACKEND_CSR_H_

/**
 * @brief standard CSR representation
 * @details
 * The data of row i is stored in data from indexPtr[i](inclusive) to indexPtr[i+1]
 * (exclusive).
 * The column indices for row i are stored in colIndex from i(inclusive),
 * i+1 (exclusive).
 */
typedef struct _ocpCSRMatFloat64Struct {
    int     nrow;     // number of rows
    int     ncol;     // number of columns, this is not necessary for data storage
    int     n;        // number of non zero values
    int*    indexPtr; // size is nrow+1, give you the range of data index for row i
    int*    colIndex; // size is n, column indices
    double* data;     // size is n, the data
} ocpCSRMatFloat64Struct;
typedef ocpCSRMatFloat64Struct* ocpCSRMatFloat64StructPtr;

#endif