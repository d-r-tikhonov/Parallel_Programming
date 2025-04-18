#include "transport_equation.h"

double courant_number(int nx, int nt)
{
    return C * nt / nx;
}

void solve_serial(double* u, int nx, int nt)
{
    double* u_new = (double*) calloc(nx + 2, sizeof(double));
    assert(u_new != NULL);

    for (int t = 0; t < nt; ++t)
    {
        for (int index = 1; index < nx; ++index)
        {
            u_new[index] = u[index] - courant_number(nx, nt) * (u[index] - u[index-1]);
        }

        // Boundary condition
        u_new[0] = 0.0;

        for (int index = 0; index < nx; ++index)
        {
            u[index] = u_new[index];
        }
    }

    free(u_new);
}

void solve_parallel(double* u, int nx, int nt, int rank, int size)
{
    int local_nx = nx / size;
    if (rank == size - 1)
    {
        local_nx += nx % size;
    }

    double* u_local = (double*) calloc((local_nx + 2), sizeof(double));

    // Initialization
    for (int index = 1; index <= local_nx; ++index) 
    {
        int global_index = rank * (nx / size) + index - 1;
        u_local[index] = sin(2 * M_PI * global_index / nx);
    }
    u_local[0] = 0.0;
    u_local[local_nx + 1] = 0.0;

    for (int t = 0; t < nt; ++t) 
    {
        // Boundary data exchange
        if (size > 1) 
        {
            int left  = (rank - 1 + size) % size;
            int right = (rank + 1) % size;

            MPI_Sendrecv(&u_local[local_nx], 1, MPI_DOUBLE, right, 0, &u_local[0],            1, MPI_DOUBLE, left,  0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Sendrecv(&u_local[1],        1, MPI_DOUBLE, left,  1, &u_local[local_nx + 1], 1, MPI_DOUBLE, right, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else 
        {
            u_local[0]            = u_local[local_nx];
            u_local[local_nx + 1] = u_local[1];
        }

       solve_serial(u_local, local_nx, nt);
    }

    // Collecting the results
    if (rank == 0) 
    {
        for (int index = 1; index <= local_nx; ++index) 
        {
            u[index - 1] = u_local[index];
        }

        for (int p = 1; p < size; p++) 
        {
            int p_local_nx = nx / size;
            if (p == size - 1) 
                p_local_nx += nx % size;
            MPI_Recv(&u[p * (nx / size)], p_local_nx, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } 
    else 
    {
        MPI_Send(&u_local[1], local_nx, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    free(u_local);
}
