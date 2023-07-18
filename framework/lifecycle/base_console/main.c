
#include "define.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int    argc;
// char** argv;

int main(int _argc, char* _argv[])
{
    ocp_info("The Basic Console program");
    // printf("The program");
    int status = 0;

    // argc = _argc;
    // argv = (char**)malloc((_argc + 1) * sizeof *argv);
    // for (int i = 0; i < argc; ++i) {
    //     size_t length = strlen(_argv[i]) + 1;
    //     argv[i]       = (char*)malloc(length);
    //     memcpy(argv[i], _argv[i], length);
    // }
    // argv[argc] = NULL;
    // for (int i = 0; i < argc; ++i) {
    //     printf("%s\n", argv[i]);
    // }

    // get command line options

    status = app(_argc, _argv);
    // post App report
    // clean up temporary files
    return 0;
}