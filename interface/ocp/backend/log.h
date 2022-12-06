
#ifndef _OCP_INTERFACE_BACKEND_LOG_H_
#define _OCP_INTERFACE_BACKEND_LOG_H_

// declare the OCPInfo function interface
typedef void (*OCPInfoType)(const char* fmt, ...);

// declare the OCPDebug function interface
typedef void (*OCPDebugType)(const char* fmt, ...);

typedef void (*OCPSetLogLevelType)(const char* lvl);

// void OCPLog_Null(const char* fmt, ...){}

#endif
