
#ifndef _OCP_INTERFACE_BACKEND_LOG_H_
#define _OCP_INTERFACE_BACKEND_LOG_H_

// declare the OCPInfo function interface
typedef void (*ocpInfoFunc)(const char* fmt, ...);

// declare the OCPDebug function interface
typedef void (*ocpDebugFunc)(const char* fmt, ...);

typedef void (*ocpSetLogLevelFunc)(const char* lvl);

// void OCPLog_Null(const char* fmt, ...){}

#endif
