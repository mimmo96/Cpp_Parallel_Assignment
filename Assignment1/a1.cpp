#include <iostream>
#include <thread>
#include <chrono>
#include "utimer.cpp"

using namespace std::chrono_literals;
using namespace std;

void body1(int n) {
  std::cout << "This is thread no. " << n << std::endl;
  //std::this_thread::sleep_for(1000ms);
  std::cout << "thread " << n << " ending now! " << std::endl; 
  return;
}

void body2(int n) {
  n = 4 + n;
  std::cout << n << " threads" << std::endl;
  return;
}

int main(int argc, char * argv[]) {

    int nthread = atoi(argv[1]);
    std::thread *t[nthread];
        
    {
        utimer u("nthread " + to_string(nthread));
        
        for(int i=0; i<nthread; i++) {
            t[i] = new std::thread(body2,i);
        }

        for(int i=0; i<nthread; i++) {
            t[i]->join();
        }
    }
    return(0);
}

//  for((i=1;i<1024;i*=2)); do ./a1 $i ;  done | grep nthread
