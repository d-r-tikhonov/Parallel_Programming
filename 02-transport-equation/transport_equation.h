#ifndef TRANPORT_EQUATION_H
#define TRANPORT_EQUATION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#ifndef NDEBUG
    #include <assert.h>
#endif

// CONSTANTS: NX - number of nodes in the space, NT - number of time steps, C - transfer rate, DX - step by x, DT - step by t

#define NX 100000
#define NT 100
#define C 1.0
#define DX 1.0
#define DT 0.5

void solve_parallel(double* u, int nx, int nt, int rank, int size);
void solve_serial(double* u, int nx, int nt);


#endif // TRANPORT_EQUATION_H
