#include <iostream>
using namespace std;

class Machine {
    string state = "q0";

public:
    void reset() { state = "q0"; }

    string transition(char input) {
        string from = state;
        if (input == '0') state = "q0";
        else if (input == '1') state = "q1";
        else state = "qDead";
        cout << "δ(" << from << ", " << input << ") → " << state << endl;
        return state;
    }

    bool run(const string& input) {
        reset();
        cout << "\nInitial State: q0\n";
        for (char ch : input)
            transition(ch);
        cout << "Final State: " << state << endl;
		return (state == "q0" && !input.empty());
    }
};

int main() {
    Machine dfa;
    string input;
    cout << "Enter binary input: ";
    cin >> input;
    if (dfa.run(input))
        cout << "Accepted: multiple of 2" << endl;
    else
        cout << "Rejected: not a multiple of 2" << endl;
    return 0;
}