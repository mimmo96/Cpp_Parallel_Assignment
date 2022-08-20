#include <iostream>
#include <thread>
#include <vector>
#include <future>  
#include <queue>
#include "threadpool.cpp"

using namespace std;


int main(int argc, char * argv[]) {

    if(argc < 5) {
        cout << "Usage is: " << argv[0] << " nw number_task arrival_time active_delay " << endl;
        return(0);
    }

    {   utimer u("pool");

        int nw = atoi(argv[1]);      // number of worked
        int nt = atoi(argv[2]);      // number of worked
        int at = atoi(argv[3]);      // arrival_time
        int ad = atoi(argv[4]);      // active_delay

        threadpool pool(nw);

        function<int()> f = [&]() { int x =1; x=x*5; active_delay(ad); return x; };
        function<int()> gg = [&]() { int x =1; x=x*5*45; active_delay(ad); return x; };

        for (int i=0; i< nt; i++){
            pool.addJob(f);
            active_delay(at);
            pool.addJob(gg);
            active_delay(at);
        }

        pool.Stop();
        
    }


    return 0;
}


//  for((i=1;i<1024;i*=2)); do ./pool $i 200 1 1 ;  done | grep pool
