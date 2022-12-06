#ifndef _OCP_INTERFACE_COMMON_H_
#define _OCP_INTERFACE_COMMON_H_
typedef enum {
    ocpOK      = 0,
    ocpWarning = 1,
    ocpError   = 2,
    ocpFatal   = 3,
} ocpStatus;

typedef char      ocpName[128];
typedef ocpName*  ocpNamePtr;
typedef char      ocpId[32];
typedef ocpId*    ocpIdPtr;
typedef char      ocpValue[32];
typedef ocpValue* ocpValuePtr;

typedef enum {
    ocpUInt,
    ocpInt,
    ocpFloat32,
    ocpFloat64,
    ocpBool,
    ocpString
} ocpDataType;
typedef ocpDataType* ocpDataTypePtr;

typedef void*       ocpContext;
typedef ocpContext* ocpContextPtr;

#endif