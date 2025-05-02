#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MAX_DEPTH 32

double* results;

typedef struct 
{
    double u_start;
    double u_end;
    double tol;
    size_t thread_index;
} thread_data_t;

double f(double u) 
{
    return sin(u) / (u * u);
}

double trapezoidal_fixed(double a, double b, int n) 
{
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));

    for (int index = 1; index < n; ++index) 
    {
        double u = a + index * h;
        sum += f(u);
    }
    return sum * h;
}

double trapezoidal_adaptive(double a, double b, double tol, int depth) 
{
    if (depth > MAX_DEPTH) 
    {
        return trapezoidal_fixed(a, b, 100);
    }

    double midpoint = (a + b) / 2;

    double first_approx  = (b - a) * (f(a) + f(b)) / 2;
    double second_approx = (b - a) / 2 * (f(a) + 2 * f(midpoint) + f(b)) / 2;
    double error = fabs(second_approx - first_approx) / 3;

    if (error < tol) 
    {
        return second_approx;
    } 
    else 
    {
        return trapezoidal_adaptive(a, midpoint, tol / 2, depth + 1) + trapezoidal_adaptive(midpoint, b, tol / 2, depth + 1);
    }
}

void* thread_func(void* arg) 
{
    thread_data_t* data = (thread_data_t*) arg;

    double result = trapezoidal_adaptive(data->u_start, data->u_end, data->tol, 0);
    results[data->thread_index] = result;

    return NULL;
}

int main(int argc, char* argv[]) 
{
    if (argc != 5) 
    {
        printf("Usage: %s <lower limit of integration> <upper integration limit> <num_threads> <tolerance>\n", argv[0]);
        return 1;
    }

    double a           = atof(argv[1]);
    double b           = atof(argv[2]);
    double num_threads = atoi(argv[3]);
    double tol_total   = atof(argv[4]);

    if (a <= 0 || b <= a) 
    {
        printf("Error: it should be 0 < a <= b!\n");
        return 1;
    }

    results = (double*) calloc(num_threads, sizeof(double));

    double delta = (b - a) / num_threads;
    double tol_thread = tol_total / num_threads;

    thread_data_t* data = (thread_data_t*) calloc(num_threads, sizeof(thread_data_t));
    pthread_t* threads = (pthread_t*) calloc(num_threads, sizeof(pthread_t));

    for (int index = 0; index < num_threads; ++index) 
    {
        data[index].u_start = 1.0 / (b - index * delta);
        data[index].u_end = 1.0 / (b - (index + 1) * delta);
        data[index].tol = tol_thread;
        data[index].thread_index = index;

        pthread_create(&threads[index], NULL, thread_func, &data[index]);
    }

    double total_integral = 0.0;
    for (int index = 0; index < num_threads; ++index) 
    {
        pthread_join(threads[index], NULL);
        total_integral += results[index];
    }

    printf("The integral sin(1/x) from %f to %f approximately equal to %f.\n", a, b, total_integral);

    free(results);
    free(data);
    free(threads);

    return 0;
}