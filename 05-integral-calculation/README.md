# Parallel Integration of sin(1/x)

This project implements a parallel numerical integration program in C to compute the definite integral of the function `sin(1/x)` over a specified interval `[a, b]` (where `0 < a < b`) using `pthread` library. The program employs an adaptive trapezoidal rule for dynamic step sizes and balances computational load across threads to ensure efficient execution.

## Features

- **Parallel Computation**: Distributes the integration task across multiple threads using `pthread` to leverage multi-core processors.
- **Adaptive Trapezoidal Rule**: Dynamically adjusts the step size based on local error estimates to achieve the desired accuracy.
- **Load Balancing**: Uses a variable substitution `u = 1/x` and non-uniform interval partitioning to ensure each thread performs approximately the same amount of work.
- **Configurable Parameters**: Allows customization of the integration limits, number of threads, and tolerance via command-line arguments.

## Compilation

To compile the program, use the following command:

```bash
gcc -o src/integral_calculation.c -lm -lpthread
```

## Usage

Run the program with optional command-line arguments to specify the integration limits, number of threads, and tolerance:

```bash
./integral [lower_limit] [upper_limit] [num_threads] [tolerance]
```

### Parameters

- `lower_limit`: Lower bound of integration. Must be positive.
- `upper_limit`: Upper bound of integration. Must satisfy `b >= a`.
- `num_threads`: Number of threads to use. Must be positive.
- `tolerance`: Total error tolerance for the integral (for example: 1e-12). Must be positive.

### Output

The program outputs the computed value of the integral, for example:
```
The integral sin(1/x) from 0.010000 to 100.000000 approximately equal to 5.027878.
```