#include <iostream>
#include <thread>
#include <chrono>
#include <future>         // std::async, std::future
#include "utimer.cpp"

using namespace std;

int body1(int n) {
  return n+3;
}

int main(int argc, char * argv[]) {

  if(argc < 3) {
    cout << "Usage is: " << argv[0] << " nthread ntimes " << endl;
    return(0);
  }

    int nthread = atoi(argv[1]);
    int ntimes = atoi(argv[2]);
    future<int> fut[nthread];

    {
        utimer u("async " + to_string(nthread));

        for(int j=0; j<ntimes; j++) {
          for(int i=0; i<nthread; i++) {
              fut[i] = std::async (launch::async, body1, nthread);
          }

          for(int i=0; i<nthread; i++) {
            try {
              std::cout << fut[i].get() << " threads" << std::endl; 
              }
            catch (const std::exception& e) {
	            cout << "Got " << e.what() << " i= " << i << endl;
            }
          }
        }
        
    }
    return(0);
}

//  for((i=1;i<1024;i*=2)); do ./async $i 100 ;  done | grep async

/*
async 1 computed in 29609 usec 
async 2 computed in 28957 usec 
async 4 computed in 17624 usec 
async 8 computed in 25239 usec 
async 16 computed in 40379 usec 
async 32 computed in 70905 usec 
async 64 computed in 145093 usec 
async 128 computed in 279956 usec 
async 256 computed in 552285 usec 
async 512 computed in 1177137 usec 
*/