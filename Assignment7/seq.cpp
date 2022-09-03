#include <iostream>
#include <chrono>
#include <vector> 

#include "utimer.cpp"

using namespace std; 


void print_vector(vector<int> n){
    cout << "\nprint vector\n ";

    for (int i = 0; i<n.size(); ++i){
        cout << n[i] << " ";
    }

    cout << endl;

}

int main(int argc, char * argv[]) {


    vector<int> v(101);

    cout << "vector generated: " ;
    v[0] = 0.0;

    for(int i=1; i<100; i++) {
        v[i] = rand() % 99;
        //v[i] = i;
    }

    v[100] = 100.0;
    
    print_vector(v);

    vector<int> temp(101);

    {
        utimer u("main ");

        for(int k=0; k<3;k++){
            for(int i=1; i<100; i++) {
                temp[i] = (v[i-1]+v[i]+v[i+1])/3;
            }
        }
    }

    
    print_vector(temp);

    return(0);
}