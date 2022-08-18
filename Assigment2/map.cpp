#include <iostream>
#include <thread>
#include <vector>
#include <future>  
#include "utimer.cpp"

using namespace std;

//mode =0 single =1 cycle sheduling

void print_vector(vector<float> n){
    cout << "\nprint vector\n ";

    for (int i = 0; i<n.size(); ++i){
        cout << n[i] << " ";
    }

}

vector<float> map(vector<float> vin, int nw, int chunk, function <float (float) > f ){

    thread *t[nw];
    int delta { (int) vin.size() / nw };

    if(chunk==0){
        future<float> fut[nw];

        for (int i = 0; i<vin.size(); ++i){
            fut[i%nw] = async (launch::async, f, vin[i] );
            vin[i]=fut[i%nw].get();
        }

        print_vector(vin);

    }
    else{
            future<void> fut[nw];
            std::vector<std::pair<float,float>> ranges(nw); // vector to compute the ranges 
            
            for(int i=0; i<nw; i++) {    // split the string into pieces
                ranges[i] = std::make_pair(i*delta, (i != (nw-1) ? (i+1)*delta - 1 : (int) vin.size() - 1));
                std::cout << "ranges[" << i << "] " << ranges[i].first << " , " << ranges[i].second << std::endl;
            }

            auto compute_chunk = [&f, &vin](auto r) {
                    for(int j=r.first; j<=r.second; j++){
                        vin[j] = f(vin[j]);
                    }
                    return ;
                    };

            for(int i=0; i<nw; i++) {    // assign chuncks to threads
                fut[i] = async (launch::async, compute_chunk, ranges[i] );
                fut[i].get();
            }
            print_vector(vin);
        
    }

    return vin;
}


int main(int argc, char * argv[]) {

    if(argc < 4) {
        cout << "Usage is: " << argv[0] << " lenght nw mode " << endl;
        return(0);
    }

    int n = atoi(argv[1]);      // vector lenght
    int nw = atoi(argv[2]);     // number of worker
    int mode = atoi(argv[3]);   // type of sheduling

    auto add_one = [](const float& x) {
		   auto r = x + 1;
		   return(r);
	};

    vector<float> v(n);

    cout << "vector generated: " ;

    for(float i=0; i<n; i++) {
        v[i] = rand() % n;
        cout << v[i] << " " ;
    }

    cout << endl;

    {
        utimer u("map");

        vector<float> result = map(v, nw , mode, add_one);
    }
    return(0);
}
