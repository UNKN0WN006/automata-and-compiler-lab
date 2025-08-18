#include <iostream>
#include <string>
using namespace std;

const char* state_names[8] = {"q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7"};
int main() {
    string bin;
    cout << "Enter a binary string: ";
    cin >> bin;

    int state = 0; // Start with 000
    cout << "Initial state: " << state_names[state] << endl;

    for (char c : bin) {
        int bit = c - '0';
        int prev_state = state;
        state = ((state << 1) | bit) & 0b111;
        cout << "delta(" << state_names[prev_state] << ", " << bit << ") = " << state_names[state] << endl;
    }

    if (state & 0b100) {
        cout << "Accepted: The 3rd last digit is 1 (ends in state " << state_names[state] << ")." << endl;
    } else {
        cout << "Rejected: The 3rd last digit is not 1 (ends in state " << state_names[state] << ")." << endl;
    }
    return 0;
}
