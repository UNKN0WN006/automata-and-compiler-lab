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
    int numZeros = j - i;

    return (j == n && numZeros == numOnes && numZeros > 1);
}

int main() {
    int n, x_len, y_len;
    cout << "Pumping Lemma Interactive Demonstrator for L = {0^n 1^n | n > 1}\n";
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

    int i_val;
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
        cout << "By the pumping lemma, this demonstrates that L is not regular.\n";
    }
    return 0;
}