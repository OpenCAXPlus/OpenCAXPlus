#ifndef _OCP_FRAMEWORK_VEC_H_
#define _OCP_FRAMEWORK_VEC_H_

/**
 * @brief simple vector
 */
typedef struct dOCPVec{
    int     nrow;     // number of rows
    double* data;     // size is nrow, the data
} dOCPVec;
typedef dOCPVec* dOCPVecPtr;

#endif