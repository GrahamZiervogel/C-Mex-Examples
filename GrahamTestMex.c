/* GrahamTestMex is a function for testing random things as I experiment with C/C++ */

#include "mex.h"

void mexFunction(int nlhs,mxArray *plhs[],int nrhs,mxArray *prhs[])
{
    #define B_OUT   plhs[0]
    #define A_IN    prhs[0]
    
    mexEvalString("x = linspace(0,5); for k = 1:200, plot(x, cos(x+k/20)); drawnow; pause(0.01); end");
    
    return;
}