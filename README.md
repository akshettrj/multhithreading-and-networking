# Operating Systems and Networks

## Assignment 5 - README

**Name:** Akshett Rai Jindal

**Roll Number:** 2019114001

# Concurrency, Parallelism and Networking

## Work Done

-   [X] Question 1 - Course Allocation Portal:
    -   [X] BONUS
-   [X] Question 2 - The Clasico Experiment:
    -   [X] BONUS
-   [X] Question 3 - Multithreaded Client and Server

## Directory Structure

    ./2019114001_assignment_5
    ├── q1
    │  ├── Makefile
    │  ├── Question1_Report.pdf
    │  ├── run_test.sh
    │  ├── src  ------------------------> Contains the code
    │  └── tests    --------------------> Contains the test inputs
    ├── q2
    │  ├── Makefile
    │  ├── Question2_Report.pdf
    │  ├── run_test.sh
    │  ├── src  ------------------------> Contains the code
    │  └── tests    --------------------> Contains the test inputs
    ├── q3
    │  ├── Makefile
    │  ├── Question3_Report.pdf
    │  ├── run_test.sh
    │  ├── src  ------------------------> Contains the code
    │  └── tests    --------------------> Contains the test inputs
    └── README.pdf

## Running the Code

### Question 1

``` sh
# Compiling the code
$ cd q1
$ make

# This produces the file: ./main

# Running the code
$ ./main
```

### Question 2

``` sh
# Compiling the code
$ cd q2
$ make

# This produces the file: ./main

# Running the code
$ ./main
```

### Question 3

``` sh
# Compiling the code
$ cd q3

# Compiling the server
$ make server

# Compiling the client
$ make client

# Compiling both at same time
$ make

# This produces the files: ./server and ./client

# Running the server
$ ./server <num_of_threads_in_pool>

# Running the clients
$ ./client
```

-   By default it uses the port 6969 to run the server
