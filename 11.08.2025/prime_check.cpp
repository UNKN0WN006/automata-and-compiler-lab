#include <iostream>
#include <cmath>
#include <string>
using namespace std;

void print_delta(const string &cur, const string &sym, const string &nxt) {
    cout << "δ(" << cur << "," << sym << ") = " << nxt << endl;
}

string stateNameFromInt(int d) {
    return to_string(d); 
}

// Simulate division of binary string by integer d, return remainder
int binaryStringMod(const string &bin, int d) {
    int rem = 0;
    for (char c : bin) {
        rem = (rem * 2 + (c - '0')) % d;
    }
    return rem;
}

// Convert binary string to integer (for sqrt limit)
int binaryStringToInt(const string &bin) {
    int val = 0;
    for (char c : bin) {
        val = val * 2 + (c - '0');
    }
    return val;
}

int main() {
    string bin;
    cout << "Enter binary string (e.g. 101 for 5): ";
    if (!(cin >> bin)) return 0;

    // Validate input
    for (char c : bin) {
        if (c != '0' && c != '1') {
            cout << "Invalid binary string." << endl;
            return 0;
        }
    }

    int n = binaryStringToInt(bin);
    if (n < 1 || n > 100) {
        cout << "Out of allowed range (1..100)." << endl;
        return 0;
    }

    string cur = "q0";
    if (n < 2) {
        print_delta(cur, "L2", "qr");
        cout << "Final state: qr\nConclusion: " << bin << " (" << n << ") is NOT PRIME\n";
        return 0;
    }
    if (n == 2) {
        print_delta(cur, "E2", "qf");
        cout << "Final state: qf\nConclusion: " << bin << " (" << n << ") is PRIME\n";
        return 0;
    }
    if (bin.back() == '0') {
        print_delta(cur, "EV", "qr");
        cout << "Final state: qr\nConclusion: " << bin << " (" << n << ") is NOT PRIME\n";
        return 0;
    }
    print_delta(cur, "OD", stateNameFromInt(3));
    int d = 3;
    int lim = (int)floor(sqrt(n));

    while (true) {
        string curState = stateNameFromInt(d);

        if (d > lim) {
            print_delta(curState, "DONE", "qf");
            cout << "Final state: qf\nConclusion: " << bin << " (" << n << ") is PRIME\n";
            break;
        }

        if (binaryStringMod(bin, d) == 0) {
            print_delta(curState, "DIV", "qr");
            cout << "Final state: qr\nConclusion: " << bin << " (" << n << ") is NOT PRIME (divisible by " << d << ")\n";
            break;
        } else {
            int nextd = d + 2;
            print_delta(curState, "NDIV", stateNameFromInt(nextd));
            d = nextd;
        }
    }
    return 0;
}