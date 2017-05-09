/* HeatEqMex.c
Solving the heat equation to demonstrate passing a function handle. */

#include "mex.h"

#define IS_REAL_2D_FULL_DOUBLE(P) ( !mxIsComplex(P) && mxGetNumberOfDimensions(P) == 2 && \
                                    !mxIsSparse(P) && mxIsDouble(P) )
#define IS_REAL_SCALAR(P) ( IS_REAL_2D_FULL_DOUBLE(P) && mxGetNumberOfElements(P) == 1 )
                                    
mxArray *g_PlotFcn;

void CallPlotFcn(mxArray *pu, mxArray *pt)
{   /* Use mexCallMATLAB to plot the current solution u */
    mxArray *ppFevalRhs[3] = {g_PlotFcn, pu, pt};
    
    mexCallMATLAB(0, NULL, 3, ppFevalRhs, "feval");     /* Call the plotfcn function handle.    */
    mexCallMATLAB(0, NULL, 0, NULL, "drawnow");         /* Call drawnow to refresh graphics.    */
}

void SolveHeatEq(mxArray *pu, double dt, size_t TimeSteps)
{   /* Crank-Nicolson method to solve u_t - u_xx = 0, u(0,t) = u(1,t) = 0 */
    mxArray *pt;
    double *u, *t, *cl, *cu, *z;
    double dx, lambda;
    size_t n, N = mxGetNumberOfElements(pu) - 1;
    
    pt = mxCreateDoubleMatrix(1, 1, mxREAL);
    u = mxGetPr(pu);
    t = mxGetPr(pt);
    u[0] = u[N] = 0.0;
    *t = 0.0;
    CallPlotFcn(pu, pt);                                /* Plot the initial condition.          */
    
    dx = 1.0/N;                                         /* Method initializations.              */
    lambda = dt/(dx*dx);
    cl = mxMalloc(sizeof(double)*N);                    /* Allocate temporary work arrays.      */
    cu = mxMalloc(sizeof(double)*N);
    z = mxMalloc(sizeof(double)*N);
    
    cl[1] = 1.0 + lambda;
    cu[1] = -lambda/(2.0*cl[1]);
    for (n = 2; n <= N-1; n++)
    {
        cl[n] = 1.0 + lambda + cu[n-1]*(lambda/2.0);
        cu[n] = -lambda/(2.0*cl[n]);
    }
    
    while (TimeSteps--)                                 /* Main computation loop.               */
    {
        z[1] = ((1.0-lambda)*u[1] + (lambda/2.0)*u[2]) / cl[1];
        for (n = 2; n <= N-1; n++)
            z[n] = ((1.0-lambda)*u[n] + (lambda/2.0)*(u[n+1] + u[n-1] + z[n-1])) / cl[n];
        for (u[N-1] = z[N-1], n = N-2; n>= 1; n--)
            u[n] = z[n] - cu[n]*u[n+1];
        
        *t += dt;
        CallPlotFcn(pu, pt);                            /* Plot the current solution.           */
    }
    
    mxFree(z);                                          /* Free work arrays.                    */
    mxFree(cu);
    mxFree(cl);
    mxDestroyArray(pt);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{   /* MEX gateway */
    #define U0_IN           prhs[0]
    #define DT_IN           prhs[1]
    #define TIMESTEPS_IN    prhs[2]
    #define PLOTFCN_IN      prhs[3]
    #define U_OUT           plhs[0]
    
    if (nrhs != 4)                                      /* Input checking.                      */
        mexErrMsgTxt("Four input arguments required.");
    else if (nlhs > 1)
        mexErrMsgTxt("Too many output arguments.");
    else if (!IS_REAL_2D_FULL_DOUBLE(U0_IN) || !IS_REAL_SCALAR(DT_IN) || !IS_REAL_SCALAR(TIMESTEPS_IN))
        mexErrMsgTxt("Invalid input.");
    else if (mxGetClassID(PLOTFCN_IN) != mxFUNCTION_CLASS && mxGetClassID(PLOTFCN_IN) != mxCHAR_CLASS)
        mexErrMsgTxt("Fourth argument should be a function handle.");
    
    U_OUT = mxDuplicateArray(U0_IN);                    /* Create output u by copying u0.       */
    g_PlotFcn = (mxArray *)PLOTFCN_IN;
    SolveHeatEq(U_OUT, mxGetScalar(DT_IN), mxGetScalar(TIMESTEPS_IN));
    
    return;
}    