# Assignments


## Assignment 1:

Evaluate overhead needed to start/join a thread and to start an async (launch::async policy) and get the result of the computed future. Measures should be taken as averages of a large number of events. Possibly, measure times on your machine AND on the remote virtual machine.

## Assignment 2:

Provide a ``` vector<float> map(vector<float>v, int mode, function<float(float)> f, int pardegree) ``` computing in parallel (using C++ threads or asyncs) the map(f) over v. Mode is 0 (block) or 1 (cyclic) scheduling.

## Assignment 3: 

Implement in parallel the odd even sort. Use as input a random generated integer vector. The odd even sort algorithm looks like the following (this is sequential; n is the length of the vector, v is the vector to sort, swap is a procedure that exchanges the two positions in the vector):

```
while(not(sorted)) {
  sorted = true;
  for(i=1; i<n-1; i+=2)
    if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
  for(i = 0; i<n-1; i+=2)
    if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
}
```

with the intent to discuss scalability of the code.

## Assignement 4:

Implement a task pool using C++ threads and STDLIB. The task pool should support parallelism degree as creation parameter and the then submission of tasks that are computed and produce results through side effects. Alternatively they have to produce a result that can be retrieved by the task submitter.

## Assignment 5:

Implementation of a farm using OpenMP. Tasks to be computed have to be provided through a for loop with iterations producing one of the input tasks and then awaiting for Ta time (parameter) before executing next iteration. The parallelism degree of the farm should be a parameter. Each task should spent some parametric amount of time (possibly using the active_wait functioN) to produce the final result.

## Assignment 6:

Write a parallel  program that finds all prime numbers in a given range of values, passed as parameters of the command line. In order to check if a number is prime, please use the following [code](http://en.wikipedia.org/wiki/Primality_test):


```
bool IsPrime(int n)
{
    if (n == 2 || n == 3)
        return true;

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    return true;
}

```

Consider your favorite parallel programming framework among the ones we've seen so far (C++ native threads, OpenMP, GrPPI) and possibly implement more that one version (e.g. a native C++ and an OpenMP version) comparing the relative performances.

## Assignment 7: 

Write a parallel program getting a vector of n floating point numbers with first position 0.0, last position 100.0 and intermediate position 25.0 and computing (in parallel) k iterations each re-computing position i in the vector (i in [1,n-2]) as the average of previous values of positions i, i-1 and i+1 (positions 0 and n never change). Two versions are required, one using FastFlow and the other one using any other framework (C++ threads, OpenMP, GrPPi).
