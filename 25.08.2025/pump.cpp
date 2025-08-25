#include <iostream>
#include <string>
using namespace std;

// Checks if a string is in L = {0^n 1^n | n > 1}
bool isInL(const string& s) {
    int n = s.length();
    if (n < 4) return false; // n > 1, so at least 2 zeros and 2 ones

    int i = 0;
    while (i < n && s[i] == '0') i++;
    int numZeros = i;
    int j = i;
    while (j < n && s[j] == '1') j++;
    int numOnes = j - i;

    return (j == n && numZeros == numOnes && numZeros > 1);
}

int main() {
    int n;
    cout << "Automated Pumping Lemma Checker for L = {0^n 1^n | n > 1}\n";
    cout << "Enter n (n > 1): ";
    cin >> n;
    if (n <= 1) {
        cout << "n must be greater than 1.\n";
        return 1;
    }

    // Build s = 0^n 1^n
    string s(n, '0');
    s += string(n, '1');
    cout << "Generated string s = " << s << " (length = " << s.length() << ")\n";

    bool foundCounterExample = false;

    // Trying all valid splits: 0 <= x_len <= n, 1 <= y_len <= n - x_len
    for (int x_len = 0; x_len <= n; ++x_len) {
        for (int y_len = 1; y_len <= n - x_len; ++y_len) {
            string x = s.substr(0, x_len);
            string y = s.substr(x_len, y_len);
            string z = s.substr(x_len + y_len);

            // Try i = 0 (remove y) and i = 2 (double y)
            for (int i_val : {0, 2}) {
            string pumped = x;
            for (int i = 0; i < i_val; ++i) pumped += y; // Start from i = 0
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
        cout << "No counterexample found. The pumping lemma did not disprove regularity for this n. \n";
        cout << "Try larger n or different splits.\n";
    }
    return 0;
}
