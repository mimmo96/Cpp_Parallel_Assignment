#include <iostream>
#include <vector>
#include <ff/ff.hpp>
#include <ff/farm.hpp>
#include <ff/pipeline.hpp>

#include "utimer.cpp"
#include "utils.cpp"

using namespace std;
using namespace ff;

typedef vector<int> myTask;  // this is my input/output type

struct Start: ff_node_t<myTask> {
    Start( myTask ve):ve(ve) {}
   myTask *svc(myTask*) {    
        for (int j=1;j<ve.size()-1;j++) {
            vector<int> new_v={ve[j-1],ve[j],ve[j+1]};
            ff_send_out(new myTask(new_v));
        }
        return EOS;
    }
    myTask ve;

};

struct Worker: ff_node_t<myTask,int> {
    int *svc(myTask*task) {
        myTask &v = *task; 
        int result = (v[0]+v[1]+v[2])/3;
        //cout << "ho ricevuto: " << v[0] <<"-"<< v[1] <<"-"<< v[2]  << endl;
        //cout << "result: " << result << endl;

        return new int(result);
    }
};

struct Stop: ff_node_t<int> {
    int *svc(int *task) {   
        int &t = *task; 
        stencil.push_back(t);
        delete task;
        return GO_ON;
    }
    void svc_end(){
        cout << "il mio lavoro qui è finito" << endl;
        //print_vector(stencil);
    
    }
    vector<int> stencil;
};

int main(int argc, char * argv[]) {

    if(argc < 3){
        cout << "usage: ./fastflow nw size" << endl;
        return 0;
    }
    
    int nw = atoi(argv[1]);
    int size = atoi(argv[2]);

    vector<int> v(size+1);

    v[0] = 0.0;

    for(int i=1; i<size; i++) {
        v[i] = rand() % (size-1);
    }

    v[size] = size;
    
    //print_vector(v);

    Start start(v);       
    Stop stop;

    {
        utimer u("fastflow ");

        std::vector<std::unique_ptr<ff_node> > W;
        for(int i=0;i<nw;++i) 
            W.push_back(make_unique<Worker>());

        ff_OFarm<> ofarm(std::move(W));

        ff_Pipe<> pipe(start, ofarm, stop);

        if (pipe.run_and_wait_end()<0)
        error("running pipe\n");
    }
    return 0;
}


/*

0 1 7 0 7 5 7 1 3 6 10 
| | | | | | | | | | | 
  | | | | | | | | |
  2 4 4 3 2 4 6 3 6 

*/