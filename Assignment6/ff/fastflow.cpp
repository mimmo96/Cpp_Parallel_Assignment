#include <iostream>
#include <vector>
#include <ff/ff.hpp>
#include <ff/farm.hpp>
#include <ff/pipeline.hpp>
#include "utimer.cpp"

using namespace std;
using namespace ff;

typedef int myTask;  // this is my input/output type

myTask is_prime(myTask n)
{
    if (n == 2 || n == 3)
        return 1;

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
        return 0;

    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }

    return 1;
}

struct Start: ff_node_t<myTask> {
    Start(int inizio, int fine):inizio(inizio), fine(fine) {}
    myTask *svc(myTask*) {    
        for (long j=inizio;j<fine;j++) {
            ff_send_out(new myTask(j));
        }
        return EOS;
    }
    int inizio;
    int fine;
};

struct Worker: ff_node_t<myTask> {
    myTask *svc(myTask *task) {
        myTask &t = *task; 
        return new myTask(is_prime(t));
    }
};

struct Stop: ff_node_t<myTask> {
    int svc_init() { conta = 0; return 0;}
    myTask *svc(myTask *task) {   
        myTask &t = *task; 
        if(t==1) conta = conta +1;
        delete task;
        return GO_ON;
    }
    void svc_end(){
        cout << "il mio lavoro qui è finito" << endl;
        cout << conta << endl;
    }
    long conta;
};

int main(int argc, char * argv[]) {

    if(argc < 4){
        cout << "usage: ./seq nw start end " << endl;
        return 0;
    }

    int nw = atoi(argv[1]);
    int inizio = atoi(argv[2]);
    int fine = atoi(argv[3]);

    Start start(inizio,fine);       
    Stop stop;

    {
        utimer u("fastflow ");

        std::vector<std::unique_ptr<ff_node> > W;
        for(int i=0;i<nw;++i) 
        W.push_back(make_unique<Worker>());

        ff_Farm<> farm(std::move(W));

        ff_Pipe<> pipe(start, farm, stop);

        if (pipe.run_and_wait_end()<0)
        error("running pipe\n");
    }
    return 0;
}
