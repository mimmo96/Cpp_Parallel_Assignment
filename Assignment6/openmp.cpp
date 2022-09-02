#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <omp.h>
#include <future>  

#include "utimer.cpp"

using namespace std; 

bool is_prime(int n)
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

int main(int argc, char * argv[]) {

    if(argc < 4){
        cout << "usage: ./seq nw start end " << endl;
        return 0;
    }

    int nw = atoi(argv[1]);
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);

    int conta = 0;

    vector<int> v(end-start);

    
    int j=0;
    
    for(int i=start; i<=end; i++) {
        v[j] = i;
        j++;
    }

    {
        utimer u("parallel ");

        #pragma parallel for num_threads(nw)
            for(int i=start; i<=end; i++) {    
               if(is_prime(i))
                    conta++;
            }

        //cout << start << " is prime? \n" << boolalpha << is_prime(start) << endl;
        cout << "number of prime number in [" << start << ","<< end << "] is : " << conta << endl;
    }

    return(0);
}