#include <iostream>
#include <string>
#include <algorithm>
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
    int n, x_len, y_len, i_val;
    cout << "Pumping Lemma Interactive Demonstrator for L = { w w^R | w in {0,1}^+ }\n";
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

    cout << "\nNow, choose a split for x, y, z such that x = s[0:x_len], y = s[x_len:x_len+y_len], z = rest.\n";
    cout << "Constraints: 0 <= x_len <= n, 1 <= y_len <= n - x_len\n";
    cout << "Enter length of x (x_len): ";
    cin >> x_len;
    if (x_len < 0 || x_len > n) {
        cout << "Invalid x length.\n";
        return 1;
    }
    cout << "Enter length of y (y_len): ";
    cin >> y_len;
    if (y_len < 1 || x_len + y_len > n) {
        cout << "Invalid y length.\n";
        return 1;
    }

    string x = s.substr(0, x_len);
    string y = s.substr(x_len, y_len);
    string z = s.substr(x_len + y_len);

    cout << "\nx = \"" << x << "\"\n";
    cout << "y = \"" << y << "\"\n";
    cout << "z = \"" << z << "\"\n";

    cout << "\nEnter i (number of times to repeat y, e.g., 0 to remove y, 2 to double y): ";
    cin >> i_val;

    string pumped = x;
    for (int i = 0; i < i_val; ++i) pumped += y;
    pumped += z;

    cout << "\nPumped string: " << pumped << endl;
    if (isInL(pumped)) {
        cout << "The pumped string IS in L.\n";
        cout << "This split does NOT prove the language is non-regular. Try other splits or values of i.\n";
    } else {
        cout << "The pumped string is NOT in L.\n";
        cout << "By the pumping lemma, this demonstrates that the language is NOT regular.\n";
    }
    return 0;
}
