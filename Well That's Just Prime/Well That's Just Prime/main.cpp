#include <iostream>
using namespace std;

class Factorizer {
    public:
        vector<int> calcPrime(int num) {
            vector<int> list;
            
            while(num % 2 == 0) {
                list.push_back(2);
                num /= 2;
            }
            
            for(int i = 3; i * i <= num; i += 2) {
                while(num % i == 0) {
                    list.push_back(i);
                    num = num / i;
                }
            }
            
            if(num > 2) {
                list.push_back(num);
            }
            
            return list;
        }
    
        void printFactors(vector<int> list) {
            for(int i : list) {
                cout << i << " ";
            }
            cout << endl;
        }
};

int main() {
    Factorizer factor;
    vector<int> f = factor.calcPrime(250);
    factor.printFactors(f);
    
    vector<int> f2 = factor.calcPrime(100);
    factor.printFactors(f2);
    
    vector<int> f3 = factor.calcPrime(24);
    factor.printFactors(f3);
}
