#include <iostream>
#include <thread>
#include <chrono>
#include <future>         // std::async, std::future
#include "utimer.cpp"

using namespace std;

void body(int n) {
  n = 4 + n;
  cout << " result:" << n << endl;
  return;
}

int main(int argc, char * argv[]) {

  if(argc < 3) {
    cout << "Usage is: " << argv[0] << " nthread ntimes " << endl;
    return(0);
  }

    int nthread = atoi(argv[1]);
    int ntimes = atoi(argv[2]);
    thread *t[nthread];
        
    {
        utimer u("nthread " + to_string(nthread));
        
        for(int j=0; j<ntimes; j++) {
          for(int i=0; i<nthread; i++) {
              t[i] = new thread(body,i);
          }

          for(int i=0; i<nthread; i++) {
              t[i]->join();
          }
        }
    }
      
    return(0);
}

//  for((i=1;i<1024;i*=2)); do ./thread $i 100 ;  done | grep nthread

/*
nthread 1 computed in 25871 usec 
nthread 2 computed in 25239 usec 
nthread 4 computed in 21872 usec 
nthread 8 computed in 63321 usec 
nthread 16 computed in 52657 usec 
nthread 32 computed in 83572 usec 
nthread 64 computed in 147919 usec 
nthread 128 computed in 277785 usec 
nthread 256 computed in 599831 usec 
nthread 512 computed in 1229091 usec 
*/