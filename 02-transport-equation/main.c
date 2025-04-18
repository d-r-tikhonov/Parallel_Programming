#include "transport_equation.h"

int main(int argc, char** argv)
{
    int rank = 0;
    int size = 0;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = 0.0;
    double end_time   = 0.0;

    if (rank == 0)
        start_time = MPI_Wtime();

    double* u = (double*) calloc(NX, sizeof(double));
    solve_parallel(u, NX, NT, rank, size);

    if (rank == 0)
    {
        end_time = MPI_Wtime();
        printf("Processors: %d, Time: %f seconds\n", size, end_time - start_time);
    }

    free(u);
    MPI_Finalize();
    return 0;
}