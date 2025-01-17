#ifndef _BSODE_H
#define _BSODE_H

#include "f_eval.h"


typedef struct _bsode
{
    int N,ISTATE;
    double EPS, HMIN, H0;
    double *Y,T;
    F_EVAL F;
} BSODE;


/*
** BSODE is a Bulirsch-Stoer method, from Press et al.
** n = number of equations.
** time = initial time.
** y = pointer to your array of n state variables.
** f = function of yours that computes ydot
** stiff_flag: 0 = no is the only option.
** eps = allowable relative error.
** zero = "problem zero"; I think it's allowable absolute error.
*/
BSODE *BsodeAlloc(int n, double t, double *y, F_EVAL f, int stiff_flag,
    double eps, double zero);

/* Do the actual integration.  Return the actual TOUT */
double BsodeIntegrate(BSODE *b, double TOUT);

void BsodeFree(BSODE *b);

#endif  /* _BSODE_H */
