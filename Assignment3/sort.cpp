#include <iostream>
#include <thread>
#include <vector>
#include <future>  
#include "utimer.cpp"
#include <barrier>

using namespace std;

vector<float> generate_vector(int n){
    vector<float> v(n);

    for(int i=0; i<n; i++) {
        v[i] = rand() % (2*n);
    }

    return v;

}

void print_vector(vector<float> n){
    cout << "vector new: ";

    for (int i = 0; i<n.size(); ++i){
        cout << n[i] << " ";
    }
    cout << endl;

}

void sequential_sort( vector<float> &v){
    bool sorted = false; 
    while(not(sorted)) {
        sorted = true;
        // Odd phase
        for(int i=1; i<v.size()-1; i+=2)
            if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
        // Even phase  
        for(int i=0; i<v.size()-1; i+=2)
            if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
    }

    print_vector(v);

    return;
}

void parallel_sort(vector<float> &v, int nw){
    
    bool sorted = false;
    bool local_sorted = false; 
    
    auto on_completion = [&]() { 
        // locking not needed here
        if(local_sorted){
            sorted=true;
            cout << "sorted " << endl;
        } 
        
    };

    barrier sync_point(nw, on_completion);
    
    auto work = [&](int i) {

        while(!sorted) { 
            local_sorted = true; 
            for(int i=1; i<v.size()-1; i+=2)
                if(v[i] > v[i+1]) { 
                    swap(v[i], v[i+1]); 
                    local_sorted = false; 
                } 
            
            //sync_point.arrive_and_wait();
            
            for(int i = 0; i<v.size()-1; i+=2)
                if(v[i] > v[i+1]) { 
                    swap(v[i], v[i+1]);
                    local_sorted = false;  
                } 
            
            sync_point.arrive_and_wait();
            
            
        }
    };
 
    vector<thread> threads;
    
        for (int i=0; i<nw; i++) {
            threads.push_back(thread(work,i));
        }

        for (auto& thread : threads) {
            thread.join();
        }
   
    print_vector(v);
    return;
}


int main(int argc, char * argv[]) {

    if(argc < 3) {
        cout << "Usage is: " << argv[0] << " lenght nw " << endl;
        return(0);
    }


    int n = atoi(argv[1]);      // vector lenght
    int nw = atoi(argv[2]);     // number of worker

    vector<float> k = generate_vector(n);
    vector<float> r = generate_vector(n);

    { 
        utimer u("sequential");
        sequential_sort(k);
    }

    { 
        utimer u("parallel");
        parallel_sort(r,nw);
    }

    return 0;
}
