#include <iostream>
#include <omp.h>
#include <chrono>
#include <thread>

using namespace std;

void offload(int task, int tw){
    #pragma omp task
    {
        this_thread::sleep_for(chrono::milliseconds(tw));
        cout << "Task " << task << " = " << task*task << endl;
    }
    return;
}

int main(int argc, char * argv[]) {
    
    int nw = 2;     //num worker 
    int ta = 1000;     //Ta time 
    int n = 10;      //num task
    int tw = 500;
    
    if(argc!=1)
        nw = atoi(argv[1]) ;
    
    //creo nw thread parallel
    #pragma omp parallel num_threads(nw) 
    {
        //indico che il mio codice dev'essere eseguito da un thread
        #pragma omp single
        {
            for (int j=0; j<n; j++) {
                int task = i;
                //eseguo in maniera asincrona
                #pragma omp task
                {
                    auto id = omp_get_thread_num();
                    cout<< "Ciao from id "<< id << endl;
                    this_thread::sleep_for(chrono::milliseconds(ta));
                    offload(task,tw);
                 }
            }
        }
        #pragma omp taskwait
    }

    return 0;
}