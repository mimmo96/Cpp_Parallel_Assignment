#include <iostream>
#include <vector>
#include <functional>

using namespace std;


void print_vector(vector<int> n){
    cout << "\nprint vector\n ";

    for (int i = 0; i<n.size(); ++i){
        cout << n[i] << " ";
    }

    cout << endl;

}