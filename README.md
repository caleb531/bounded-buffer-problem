# CS 433 Bounded Buffer Problem

*Copyright 2017 Caleb Evans, Jonathan Tapia*

This program implements a multithreaded C++ solution to the Bounded Buffer
Problem (also known as the Producer-Consumer Problem).

## How to run

Run `make` to compile the program, then execute the compiled program by running
`./bbp.out` with three arguments. The first argument is the number of seconds
the program will sleep before timing out and terminating. The second argument is
the number of producer threads to create. The third and final argument is the
number of consumer threads to create. For instance:

```bash
# Create 2 producers and 3 consumers, timing out after 10 seconds
./bbp.out 10 2 3
```

When a producer inserts a value or when a consumer removes a value, the
respective thread will display the operation, value, and new buffer contents.
For example:

```
produce 5 => [5]
consume 5 => []
produce 3 => [3]
produce 8 => [3, 8]
```
