#ifndef _SDRIV2_H
#define _SDRIV2_H

#include "f_eval.h"

/*
** SDRIV2 is an Adams method documented in _Numerical Methods and
** Software_, by Kahaner, Moler, and Nash.
** n = number of equations.
** time = initial time.
** y = pointer to your array of n state variables.
** f = function of yours that computes ydot
** stiff_flag: 0 = no, 1 = yes, 2 = dynamically decided by SDRIV2
** eps = allowable relative error.
** zero = "problem zero"; I think it's allowable absolute error.
*/
void init_sdriv2(int n, double time, double *y, F_EVAL f, int stiff_flag, double eps, double zero);

/* Do the actual integration.  Return the actual TOUT */
double integrate_sdriv2(double TOUT);

#endif  /* _SDRIV2_H */
