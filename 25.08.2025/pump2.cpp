#include <iostream>
#include <string>
#include<algorithm>
using namespace std;

// Checks if s is of the form w w^R for non-empty w
bool isInL(const string& s) {
    int n = s.length();
    if (n < 2 || n % 2 != 0) return false; // Must be even and at least length 2

    int half = n / 2;
    string w = s.substr(0, half);
    string wR = s.substr(half, half);

    // Check if wR is the reverse of w
    for (int i = 0; i < half; ++i) {
        if (w[i] != wR[half - 1 - i]) return false;
    }
    return true;
}

int main() {
    int n;
    cout << "Automated Pumping Lemma Checker for L = { w w^R | w in {0,1}^+ }\n";
    cout << "Enter n (n > 0, w will have length n): ";
    cin >> n;
    if (n <= 0) {
        cout << "n must be greater than 0.\n";
        return 1;
    }

    // Build s = w w^R, where w = n times '0'
    string w(n, '0');
    string wR = w;
    reverse(wR.begin(), wR.end());
    string s = w + wR;
    cout << "Generated string s = " << s << " (length = " << s.length() << ")\n";

    bool foundCounterExample = false;
    int p = n; // Use n as the pumping length

    // Try all valid splits: 0 <= x_len <= p, 1 <= y_len <= p - x_len
    for (int x_len = 0; x_len <= p; ++x_len) {
        for (int y_len = 1; y_len <= p - x_len; ++y_len) {
            string x = s.substr(0, x_len);
            string y = s.substr(x_len, y_len);
            string z = s.substr(x_len + y_len);

            // Try i = 0 (remove y) and i = 2 (double y)
            for (int i_val : {0, 2}) {
                string pumped = x;
                for (int i = 0; i < i_val; ++i) pumped += y;
                pumped += z;

                if (!isInL(pumped)) {
                    cout << "\nCounterexample found!\n";
                    cout << "Split: x = \"" << x << "\", y = \"" << y << "\", z = \"" << z << "\"\n";
                    cout << "Pumping y " << i_val << " times gives: " << pumped << endl;
                    cout << "This string is NOT in L.\n";
                    cout << "Therefore, by the pumping lemma, L is NOT regular.\n";
                    foundCounterExample = true;
                    break;
                }
            }
            if (foundCounterExample) break;
        }
        if (foundCounterExample) break;
    }

    if (!foundCounterExample) {
        cout << "No counterexample found. The pumping lemma did not disprove regularity for this n.\n";
        cout << "Try larger n or different splits.\n";
    }
    return 0;
}
