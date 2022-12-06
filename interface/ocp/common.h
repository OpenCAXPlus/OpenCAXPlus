typedef enum {
    ocpOK      = 0,
    ocpWarning = 1,
    ocpError   = 2,
    ocpFatal   = 3,
} ocpStatus;

typedef char ocpName[128];

typedef void*       ocpContext;
typedef ocpContext* ocpContextPtr;

// typedef void* ocpCallback;

typedef ocpStatus    ocpCallback(ocpContext a);
typedef ocpCallback* ocpCallbackPtr;
