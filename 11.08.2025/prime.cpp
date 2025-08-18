#include <iostream>
#include <string>
#include <cmath>
using namespace std;

int main() {
    string bin;
    cout << "Enter a binary string: ";
    cin >> bin;

    if (bin == "0" || bin == "1") {
        cout << "The binary string does NOT represent a prime number (0 or 1)." << endl;
        return 0;
    }

    if (bin.back() == '0' && bin != "10") { 
        cout << "Divisor: 2" << endl;
        int rem = 0;
        for (char c : bin) {
            int bit = c - '0';
            int prev_rem = rem;
            rem = (rem * 2 + bit) % 2;
            cout << "delta(r=" << prev_rem << ", " << bit << ") = " << rem << endl;
        }
        cout << "The binary string does NOT represent a prime number (even number)." << endl;
        return 0;
    }

    int n = 0;
    for (char c : bin) n = n * 2 + (c - '0');
    int sqrtN = (int) sqrt(n);

    bool isPrime = true;
    for (int d = 3; d <= sqrtN; d += 2) {
        cout << "Divisor: " << d << endl;
        int rem = 0;
        for (char c : bin) {
            int bit = c - '0';
            int prev_rem = rem;
            rem = (rem * 2 + bit) % d;
            cout << "delta(r=" << prev_rem << ", " << bit << ") = " << rem << endl;
        }
        if (rem == 0) {
            cout << "The binary string does NOT represent a prime number (divisible by " << d << ")." << endl;
            isPrime = false;
            break;
        }
    }
    if (isPrime) {
        cout << "The binary string represents a prime number." << endl;
    }
    return 0;
}