/* NORMALIZECOLSMEX.C	Normalize the columns of a matrix
Syntax:		B = normalizecols(A)
		or	B = normalizecols(A,p)
The columns of matrix A are normalized so that norm(B(:,n),p) = 1. */
#include <math.h>
#include "mex.h"

#define IS_REAL_2D_FULL_DOUBLE(P) (!mxIsComplex(P) && \
mxGetNumberOfDimensions(P) == 2 && !mxIsSparse(P) && mxIsDouble(P))
#define IS_REAL_SCALAR(P) (IS_REAL_2D_FULL_DOUBLE(P) && mxGetNumberOfElements(P) == 1)

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	/* Macros for the output and input arguments */
	#define B_OUT		plhs[0]
	#define A_IN		prhs[0]
	#define P_IN		prhs[1]
	double *B, *A, p, colnorm;
	int M, N, m, n;
	
	if (nrhs < 1 || nrhs > 2) /* Check the number of arguments */
		mexErrMsgTxt("Wrong number of input arguments.");
	else if (nlhs > 1)
		mexErrMsgTxt("Too many output arguments.");
	
	if (!IS_REAL_2D_FULL_DOUBLE(A_IN)) /* Check A */
		mexErrMsgTxt("A must be a real 2D full double array.");
		
	if (nrhs == 1) /* If p is unspecified, set it to a default value */
		p = 2.0;
	else /* If p was specified, check that it is a real double scalr */
		if (!IS_REAL_SCALAR(P_IN))
			mexErrMsgTxt("P must be a real double scalar.");
		else
			p = mxGetScalar(P_IN); /* Get p */
		
	M = mxGetM(A_IN);		/* Get the dimensions of A */
	N = mxGetN(A_IN);
	A = mxGetPr(A_IN); 		/* Get the pointer of the data of A */
	B_OUT = mxCreateDoubleMatrix(M,N,mxREAL); /* Create the output matrix */
	B = mxGetPr(B_OUT);		/* Get the pointer to the data of B */
	
	for (n = 0; n < N; n++) 	/* Compute a matrix with normalized columns */
	{
		for (m = 0, colnorm = 0.0; m < M; m++) colnorm += pow(A[m + M*n], p);
		colnorm = pow(fabs(colnorm),1.0/p);	/* Compute the norm of the nth column */
		
		for (m = 0; m < M; m++) B[m + M*n] = A[m + M*n]/colnorm;
	}
	
	return;
}