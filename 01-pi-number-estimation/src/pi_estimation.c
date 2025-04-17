#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_ITERATIONS_AMOUNT 1e6

int main(int argc, char** argv)
{
    int rank = 0;
    int size = 0;
 
    double pi = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();

    if (size < 2)
    {
        if (rank == 0)
        {
            printf("Error! The program needs at least 2 processes to work...\n");
        }

        MPI_Finalize();
        return 1;
    }

    double step = 1.0 / (double)MAX_ITERATIONS_AMOUNT;
    double x    = 0.0;
    double sum  = 0.0;

    for (size_t index = rank; index < MAX_ITERATIONS_AMOUNT; index += size)
    {
        x = (index + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double global_sum = 0.0;
    MPI_Reduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        pi = global_sum * step;

        double end_time = MPI_Wtime();

        printf("Calculated Pi = %.16f\n", pi);
        printf("Error = %.16f\n", pi - 3.14159265358979323846);
        printf("Execution time = %.4f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}