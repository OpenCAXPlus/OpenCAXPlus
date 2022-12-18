#ifndef _OCP_INTERFACE_COMMON_H_
#define _OCP_INTERFACE_COMMON_H_
typedef enum {
    OCP_OK      = 0,
    OCP_WARNING = 1,
    OCP_ERROR   = 2,
    OCP_FATAL   = 3,
} ocpStatusEnum;

typedef char              ocpString32Type[32];
typedef char              ocpString128Type[128];
typedef ocpString32Type*  ocpString32TypePtr;
typedef ocpString128Type* ocpString128TypePtr;

typedef enum {
    OCP_UINT,
    OCP_INT,
    OCP_FLOAT32,
    OCP_FLOAT64,
    OCP_BOOL,
    OCP_STRING32,
    OCP_STRING128
} ocpDataEnum;
typedef ocpDataEnum* ocpDataEnumPtr;

#endif