#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX_MESSAGE_SIZE 128
#define MAX_ITERATIONS_AMOUNT 512
#define TAG 0

int main(int argc, char** argv)
{
  int rank = 0;
  int size = 0;

  double start_time = 0.0;
  double end_time   = 0.0;
  double total_time = 0.0;

  char message[MAX_MESSAGE_SIZE] = "";

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 2)
  {
    if (rank == 0)
    {
        printf("Error! The program needs at least 2 processes to work...\n");
    }

    MPI_Finalize();
    return 1;
  }

  for (int index = 0; index < MAX_ITERATIONS_AMOUNT; ++index)
  {
    if (rank == 0)
    {
        start_time = MPI_Wtime();
        MPI_Send(message, MAX_MESSAGE_SIZE, MPI_CHAR, 1, TAG, MPI_COMM_WORLD);
        MPI_Recv(message, MAX_MESSAGE_SIZE, MPI_CHAR, 1, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end_time   = MPI_Wtime();

        total_time += (end_time-start_time);
    }
    else if (rank == 1)
    {
        MPI_Recv(message, MAX_MESSAGE_SIZE, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(message, MAX_MESSAGE_SIZE, MPI_CHAR, 0, TAG, MPI_COMM_WORLD);
    }
  }

  if (rank == 0)
  {
    printf("Average round trip communication time: %.3f mcs\n", total_time / (2 * MAX_ITERATIONS_AMOUNT) * 1e6);
    printf("Message size: %ld byte\n", sizeof(message));
    printf("Iterations amount: %d\n", MAX_ITERATIONS_AMOUNT);
  }

  MPI_Finalize();
  return 0;
}