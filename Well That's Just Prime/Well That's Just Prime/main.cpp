#include <iostream>
#include <vector>
using namespace std;

class Factorizer {
public:
    vector<int> primeFactors(int num, int div = 2) {
        if(num <= 1) {
            return {};
        }
        
        if(num % div == 0) {
            vector<int> factors = {div};
            vector<int> restFactors = primeFactors(num / div, div);
            
            factors.insert(factors.end(), restFactors.begin(), restFactors.end());
            return factors;
        } else {
            return primeFactors(num, div + 1);
        }
    }
};

int main() {
    Factorizer factorizer;
    int num;
    
    cout << "Enter a number: ";
    cin >> num;
    vector<int> factors = factorizer.primeFactors(num);
    
    cout << "Your list of prime factor(s): " << endl;
    for(int i: factors) {
        cout << i << endl;
    }
}
