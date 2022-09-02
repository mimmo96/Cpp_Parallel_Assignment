#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
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

    int count = 0;

    vector<int> v(end-start);

    
    int j=0;
    
    for(int i=start; i<=end; i++) {
        v[j] = i;
        j++;
    }

    {
        utimer u("parallel ");

        atomic <int> conta(0);

        int delta { (int) v.size() / nw };

        vector<thread> threads;
        std::vector<std::pair<float,float>> ranges(nw); // vector to compute the ranges 
            
        for(int i=0; i<nw; i++) {    // split the string into pieces
            ranges[i] = std::make_pair(i*delta, (i != (nw-1) ? (i+1)*delta - 1 : (int) v.size() - 1));
        }

        auto compute_chunk = [&conta, &v](auto r) {
                for(int j=r.first; j<=r.second; j++){
                    if(is_prime(v[j])) conta ++;
                }
                return ;
        };

        for(int i=0; i<nw; i++) {    // assign chuncks to threads
            threads.emplace_back( compute_chunk, ranges[i] );
           
        }

        for(auto& thread : threads)
            thread.join();

        //cout << start << " is prime? \n" << boolalpha << is_prime(start) << endl;
        cout << "number of prime number in [" << start << ","<< end << "] is : " << conta << endl;
    }

    return(0);
}