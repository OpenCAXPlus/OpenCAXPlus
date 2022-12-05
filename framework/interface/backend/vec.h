#ifndef _OCP_FRAMEWORK_VEC_H_
#define _OCP_FRAMEWORK_VEC_H_

/**
 * @brief simple vector
 */
typedef struct _ocpVecFloat64{
    int     nrow;     // number of rows
    double* data;     // size is nrow, the data
} ocpVecFloat64
;
typedef ocpVecFloat64* ocpVecFloat64Ptr;

#endif