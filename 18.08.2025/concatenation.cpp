#include <iostream>
#include <vector>
#include <set>
#include <string>
using namespace std;

int binToInt(const string& s) {
    int n = 0;
    for (char c : s) n = n * 2 + (c - '0');
    return n;
}

vector<string> generateBinaryLessThan(int maxVal) {
    vector<string> res;
    for (int i = 0; i < maxVal; ++i) {
        string s;
        int x = i;
        if (x == 0) s = "0";
        else {
            while (x > 0) {
                s = char('0' + (x % 2)) + s;
                x /= 2;
            }
        }
        res.push_back(s);
    }
    return res;
}

int main() {
    int maxVal;
    cout << "Enter the upper bound (exclusive) for binary numbers (e.g., 10 for <10): ";
    cin >> maxVal;
    vector<string> all = generateBinaryLessThan(maxVal);

    set<string> A, B;
    cout << "Set A: Even binary numbers less than " << maxVal << endl;
    for (const string& s : all) {
        if (s.back() == '0') A.insert(s);
    }
    cout << "Set B: Odd binary numbers less than " << maxVal << endl;
    for (const string& s : all) {
        if (s.back() == '1') B.insert(s);
    }

    // M(A U B)
    set<string> unionSet = A;
    unionSet.insert(B.begin(), B.end());
    cout << "\nM(A U B): { ";
    int count = 0;
    for (const string& s : unionSet) {
        if (count < 10) cout << s << " ";
        count++;
    }
    if (count > 10) cout << "... ";
    cout << "}" << endl;

    // M(A • B)
    set<string> concatSet;
    for (const string& a : A) {
        for (const string& b : B) {
            concatSet.insert(a + b);
        }
    }
    cout << "M(A • B): { ";
    count = 0;
    for (const string& s : concatSet) {
        if (count < 10) cout << s << " ";
        count++;
    }
    if (count > 10) cout << "... ";
    cout << "}" << endl;

    // M(A*)
    set<string> starSet;
    starSet.insert(""); // epsilon
    // Generating up to 2 concatenations for demonstration
    for (const string& a : A) if (!a.empty()) starSet.insert(a);
    for (const string& a1 : A) for (const string& a2 : A) if (!a1.empty() && !a2.empty()) starSet.insert(a1 + a2);
    cout << "M(A*): { ";
    count = 0;
    for (const string& s : starSet) {
        if (count < 10) cout << (s.empty() ? "ε" : s) << " ";
        count++;
    }
    cout << "... }" << endl;

    return 0;
}
