#include "App.h"
int App(int argc, const char* argv[])
{
    //---------------------//
    // Step 0. Preparation //
    //---------------------//

    // Set parameters
    input_param inipar; // parameters from input files
    ITS_param   itspar; // parameters for itsolver
    AMG_param   amgpar; // parameters for AMG
    ILU_param   ilupar; // parameters for ILU
    SWZ_param   swzpar; // parameters for Schwarz method

    // Set solver parameters
    fasp_param_set(argc, argv, &inipar);
    fasp_param_init(&inipar, &itspar, &amgpar, &ilupar, &swzpar);

    // Set local parameters
    const int print_level  = inipar.print_level;
    const int problem_num  = inipar.problem_num;
    const int solver_type  = inipar.solver_type;
    const int precond_type = inipar.precond_type;
    const int output_type  = inipar.output_type;

    // Set output device
    if (output_type) {
        char* outputfile = "out/test.out";
        printf("Redirecting outputs to file: %s ...\n", outputfile);
        if (freopen(outputfile, "w", stdout) == NULL) // open a file for stdout
            fprintf(stderr, "Output redirecting stdout\n");
    }

    // Data filenames
    char filename1[512], *datafile1;
    char filename2[512], *datafile2;


    memcpy(filename1, inipar.workdir, 256);
    memcpy(filename2, inipar.workdir, 256);


    // Matrix and vector
    dCSRmat A;
    dvector b, x;
    int     status = FASP_SUCCESS;

    //----------------------------------------------------//
    // Step 1. Input stiffness matrix and right-hand side //
    //----------------------------------------------------//
    printf("Test Problem Number %d\n", problem_num);

    if (problem_num == 10) {
        // Read A and b -- P1 FE discretization for Poisson.
        datafile1 = "csrmat_FE.dat";
        strcat(filename1, datafile1);

        datafile2 = "rhs_FE.dat";
        strcat(filename2, datafile2);

        fasp_dcsrvec_read2(filename1, filename2, &A, &b);
    }

    // Print problem size
    if (print_level > PRINT_NONE) {
        printf("A: m = %d, n = %d, nnz = %d\n", A.row, A.col, A.nnz);
        printf("b: n = %d\n", b.row);
    }

    // Print out solver parameters
    if (print_level > PRINT_NONE) fasp_param_solver_print(&itspar);

    //--------------------------//
    // Step 2. Solve the system //
    //--------------------------//

    // Set initial guess
    fasp_dvec_alloc(A.row, &x);
    fasp_dvec_set(A.row, &x, 0.0);

    status = fasp_solver_dcsr_krylov(&A, &b, &x, &itspar);

    if (status < 0) {
        printf("\n### ERROR: Solver failed! Exit status = %d.\n\n", status);
    }

    if (output_type) fclose(stdout);

    //------------------------//
    // Step 3. Check solution //
    //------------------------//
    fasp_blas_dcsr_aAxpy(-1.0, &A, x.val, b.val); // compute residual

    printf("L2 norm of residual = %.10e\n\n", fasp_blas_dvec_norm2(&b));

    //-------------------------//
    // Step 4. Clean up memory //
    //-------------------------//
    fasp_dcsr_free(&A);
    fasp_dvec_free(&b);
    fasp_dvec_free(&x);

    return status;
}

/*---------------------------------*/
/*--        End of File          --*/
/*---------------------------------*/