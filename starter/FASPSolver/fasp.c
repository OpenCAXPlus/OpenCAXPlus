/*! \file  test.c
 *
 *  \brief The main test function for FASP solvers
 *
 *---------------------------------------------------------------------------------
 *  Copyright (C) 2009--Present by the FASP team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *---------------------------------------------------------------------------------
 */

#include "fasp.h"
#include "fasp_functs.h"
#include "global.h"

// Set parameters
input_param inipar; // parameters from input files
ITS_param   itspar; // parameters for itsolver
AMG_param   amgpar; // parameters for AMG
ILU_param   ilupar; // parameters for ILU
SWZ_param   swzpar; // parameters for Schwarz method

const int print_level;
const int problem_num;
const int solver_type;
const int precond_type;
const int output_type;

// Matrix and vector
dCSRmat A;
dvector b, x;
int     status = FASP_SUCCESS;

int InputRead()
{
    // Set solver parameters
    fasp_param_set(argc, (const char**)argv, &inipar);
    fasp_param_init(&inipar, &itspar, &amgpar, &ilupar, &swzpar);

    // Set local parameters
    const int print_level  = inipar.print_level;
    const int problem_num  = inipar.problem_num;
    const int solver_type  = inipar.solver_type;
    const int precond_type = inipar.precond_type;
    const int output_type  = inipar.output_type;
}

int SimulationSetup()
{
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

    strncpy(filename1, inipar.workdir, 128);
    strncpy(filename2, inipar.workdir, 128);
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

    else if (problem_num == 11) {
        // Read A -- P1 FE discretization for Poisson, 1M DoF
        datafile1 = "Poisson/coomat_1046529.dat"; // This file is NOT in ../data!
        strcat(filename1, datafile1);
        fasp_dcoo_read(filename1, &A);

        // Generate a random solution
        dvector sol = fasp_dvec_create(A.row);
        fasp_dvec_rand(A.row, &sol);

        // Form the right-hand-side b = A*sol
        b = fasp_dvec_create(A.row);
        fasp_blas_dcsr_mxv(&A, sol.val, b.val);
        fasp_dvec_free(&sol);
    }

    else if (problem_num == 12) {
        // Read A -- P1 FE discretization for Poisson, 0.25M DoF
        datafile1 = "Poisson/coomat_261121.dat"; // This file is NOT in ../data!
        strcat(filename1, datafile1);
        fasp_dcoo_read(filename1, &A);

        // Generate a random solution
        dvector sol = fasp_dvec_create(A.row);
        fasp_dvec_rand(A.row, &sol);

        // Form the right-hand-side b = A*sol
        b = fasp_dvec_create(A.row);
        fasp_blas_dcsr_mxv(&A, sol.val, b.val);
        fasp_dvec_free(&sol);
    }

    else {
        printf("### ERROR: Unrecognised problem number %d\n", problem_num);
        return ERROR_INPUT_PAR;
    }

    // Print problem size
    if (print_level > PRINT_NONE) {
        printf("A: m = %d, n = %d, nnz = %d\n", A.row, A.col, A.nnz);
        printf("b: n = %d\n", b.row);
    }

    // Print out solver parameters
    if (print_level > PRINT_NONE) fasp_param_solver_print(&itspar);
}

int SimulationRun()
{

    //--------------------------//
    // Step 2. Solve the system //
    //--------------------------//

    // Set initial guess
    fasp_dvec_alloc(A.row, &x);
    fasp_dvec_set(A.row, &x, 0.0);

    // Preconditioned Krylov methods
    if (solver_type >= 1 && solver_type <= 20) {

        // Using no preconditioner for Krylov iterative methods
        if (precond_type == PREC_NULL) {
            status = fasp_solver_dcsr_krylov(&A, &b, &x, &itspar);
        }

        // Using diag(A) as preconditioner for Krylov iterative methods
        else if (precond_type == PREC_DIAG) {
            status = fasp_solver_dcsr_krylov_diag(&A, &b, &x, &itspar);
        }

        // Using AMG as preconditioner for Krylov iterative methods
        else if (precond_type == PREC_AMG || precond_type == PREC_FMG) {
            if (print_level > PRINT_NONE) fasp_param_amg_print(&amgpar);
            status = fasp_solver_dcsr_krylov_amg(&A, &b, &x, &itspar, &amgpar);
        }

        // Using ILU as preconditioner for Krylov iterative methods Q: Need to change!
        else if (precond_type == PREC_ILU) {
            if (print_level > PRINT_NONE) fasp_param_ilu_print(&ilupar);
            status = fasp_solver_dcsr_krylov_ilu(&A, &b, &x, &itspar, &ilupar);
        }

        // Using Schwarz as preconditioner for Krylov iterative methods
        else if (precond_type == PREC_SCHWARZ) {
            if (print_level > PRINT_NONE) fasp_param_swz_print(&swzpar);
            status = fasp_solver_dcsr_krylov_swz(&A, &b, &x, &itspar, &swzpar);
        }

        else {
            printf("### ERROR: Unknown preconditioner %d!!!\n", precond_type);
            status = ERROR_SOLVER_PRECTYPE;
        }
    }

    // AMG as the iterative solver
    else if (solver_type == SOLVER_AMG) {
        if (print_level > PRINT_NONE) fasp_param_amg_print(&amgpar);
        fasp_solver_amg(&A, &b, &x, &amgpar);
    }

    // Full AMG as the iterative solver
    else if (solver_type == SOLVER_FMG) {
        if (print_level > PRINT_NONE) fasp_param_amg_print(&amgpar);
        fasp_solver_famg(&A, &b, &x, &amgpar);
    }

#if WITH_SuperLU // Call SuperLU if linked with -DUSE_SUPERLU=ON
    else if (solver_type == SOLVER_SUPERLU) {
        status = fasp_solver_superlu(&A, &b, &x, print_level);
    }
#endif

#if WITH_UMFPACK // Call UMFPACK if linked with -DUSE_UMFPACK=ON
    else if (solver_type == SOLVER_UMFPACK) {
        dCSRmat A_tran;
        fasp_dcsr_trans(&A, &A_tran);
        fasp_dcsr_sort(&A_tran);
        void* Numeric = fasp_umfpack_factorize(&A_tran, print_level);

        status = fasp_umfpack_solve(&A_tran, &b, &x, Numeric, print_level);
        fasp_umfpack_free_numeric(Numeric);
        fasp_dcsr_free(&A_tran);
    }
#endif

#if WITH_MUMPS // Call MUMPS if linked with -DUSE_MUMPS=ON
    else if (solver_type == SOLVER_MUMPS) {
        status = fasp_solver_mumps(&A, &b, &x, print_level);
    }
#endif

#if WITH_PARDISO // Call PARDISO if linked with -DUSE_PARDISO=ON
    else if (solver_type == SOLVER_PARDISO) {
        fasp_dcsr_sort(&A);
        status = fasp_solver_pardiso(&A, &b, &x, print_level);
    }
#endif

#if WITH_STRUMPACK // Call STRUMPACK if linked with -DUSE_STRUMPACK=ON
    else if (solver_type == SOLVER_STRUMPACK) {
        fasp_dcsr_sort(&A);
        status = fasp_solver_strumpack(&A, &b, &x, print_level);
    }
#endif

    else {
        printf("### ERROR: Unknown solver %d!!!\n", solver_type);
        status = ERROR_SOLVER_TYPE;
    }

    if (status < 0) {
        printf("\n### ERROR: Solver failed! Exit status = %d.\n\n", status);
    }

    if (output_type) fclose(stdout);

    //------------------------//
    // Step 3. Check solution //
    //------------------------//

    return status;
}

int ResultOutput() {}

int Cleanup()
{
    //-------------------------//
    // Step 4. Clean up memory //
    //-------------------------//
    fasp_dcsr_free(&A);
    fasp_dvec_free(&b);
    fasp_dvec_free(&x);
}
