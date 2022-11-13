#include "log.h"
#ifdef __cplusplus
extern "C" {
#endif
void print(const char* tp, const char* fmt, ...)
{
    va_list args;
    char    temp[512] = {'\0'};
    sprintf(temp, "%s: %s\n", tp, fmt);
    va_start(args, fmt);
    vprintf(temp, args);
    va_end(args);
}

void LoggerSetup(){
    
}
#ifdef __cplusplus
}
#endif