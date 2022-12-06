
#ifndef _OCP_INTERFACE_BACKEND_LOG_H_
#define _OCP_INTERFACE_BACKEND_LOG_H_

// declare the OCPInfo function interface
typedef void         OCPInfoType(const char* fmt, ...);
typedef OCPInfoType* OCPInfoTypePtr;

// declare the OCPDebug function interface
typedef void          OCPDebugType(const char* fmt, ...);
typedef OCPDebugType* OCPDebugTypePtr;

typedef void                OCPSetLogLevelType(const char* lvl);
typedef OCPSetLogLevelType* OCPSetLogLevelTypePtr;

// void OCPLog_Null(const char* fmt, ...){}

#endif
