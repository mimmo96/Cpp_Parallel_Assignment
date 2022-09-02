#include <iostream>
#include <chrono>

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

    if(argc < 3){
        cout << "usage: ./seq start end " << endl;
        return 0;
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);
    int count = 0;

    {
        utimer u("main ");

        for (int i=start; i<=end; i++){
            if (is_prime(i)) count++;
        }
        //cout << start << " is prime? \n" << boolalpha << is_prime(start) << endl;
        cout << "number of prime number in [" << start << ","<< end << "] is : " << count << endl;
    
    }

    return(0);
}