#ifndef _OCP_INTERFACE_BACKEND_VEC_H_
#define _OCP_INTERFACE_BACKEND_VEC_H_

/**
 * @brief simple vector
 */
typedef struct _ocpVecFloat64Struct {
    int     nrow; // number of rows
    double* data; // size is nrow, the data
} ocpVecFloat64Struct;
typedef ocpVecFloat64Struct* ocpVecFloat64StructPtr;

#endif