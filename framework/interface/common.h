typedef enum {
    ocpOK,
    ocpWarning,
    ocpError,
    ocpFatal,
} ocpStatus;

typedef char ocpName[128];

typedef void* ocpContext;
typedef ocpContext* ocpContextPtr;

// typedef void* ocpCallback;

typedef ocpStatus  ocpCallback(ocpContext a);
typedef ocpCallback* ocpCallbackPtr;
