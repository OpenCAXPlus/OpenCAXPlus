#include "solver.h"


ocpStatusEnum fasp_CSR_solver(ocpCSRSolverContextStructPtr context)
{
    ocp_info("Using the fasp csr solver through OCP wrapper");
    // convert OCPCSRMat format to the fasp CSR format
    dCSRmat faspCSR;
    faspCSR.col = context->A->ncol;
    faspCSR.row = context->A->nrow;
    faspCSR.nnz = context->A->n;
    faspCSR.val = context->A->data;
    faspCSR.IA  = context->A->indexPtr;
    faspCSR.JA  = context->A->colIndex;

    // CSR_transform();
    // vector_transform();
    dvector b;
    b.row = context->b->nrow;
    b.val = context->b->data;

    dvector x;
    x.row = context->x->nrow;
    x.val = context->x->data;

    ITS_param itspar;
    itspar.print_level   = 0;
    itspar.itsolver_type = SOLVER_DEFAULT; /**< solver type: see fasp_const.h */
    itspar.precond_type  = 1;              /**< preconditioner type */
    itspar.stop_type     = 1;              /**< stopping type */
    itspar.restart       = 1;    /**< number of steps for restarting: for GMRES etc */
    itspar.maxit         = 100;  /**< max number of iterations */
    itspar.tol           = 0.01; /**< convergence tolerance for relative residual */


    // call fasp CSR solve
    fasp_solver_dcsr_krylov(&faspCSR, &b, &x, &itspar);
}