#include <iostream>
#include <string>
using namespace std;

const char* state_names[6] = {"q0", "q1", "q2", "q3", "q4", "q5"};
int get_state(int rem3, int rem2) {
    return rem3 * 2 + rem2;
}
int main() {
    string bin;
    cout << "Enter a binary string: ";
    cin >> bin;

    int rem3 = 0, rem2 = 0;
    int state = get_state(rem3, rem2);
    cout << "Initial state: " << state_names[state] << endl;

    for (char c : bin) {
        int bit = c - '0';
        int prev_rem3 = rem3, prev_rem2 = rem2, prev_state = state;

        rem3 = (rem3 * 2 + bit) % 3;
        rem2 = (rem2 * 2 + bit) % 2;
        state = get_state(rem3, rem2);
        cout << "delta(" << state_names[prev_state] << ", " << bit << ") = " << state_names[state] << endl;
    }

    if (rem3 == 0 && rem2 == 0) {
        cout << "The binary string is divisible by 6 (ends in state " << state_names[state] << ")." << endl;
    } else {
        cout << "The binary string is NOT divisible by 6 (ends in state " << state_names[state] << ")." << endl;
    }
    return 0;
}
