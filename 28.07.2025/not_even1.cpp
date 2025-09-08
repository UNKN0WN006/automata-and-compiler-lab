#include <iostream>
using namespace std;

class not_even1 {
    string state = "q0";

public:
    void reset() { state = "q0"; }

    string transition(char input) {
        string from = state;
        if (state == "q0") state = (input == '1') ? "q1" : "q0";
        else if (state == "q1") state = (input == '1') ? "q0" : "q1";

        cout << "δ(" << from << ", " << input << ") → " << state << endl;
        return state;
    }

    bool run(const string& input) {
        reset();
        cout << "\nInitial State: q0\n";

        for (char ch : input) {
            if (ch != '0' && ch != '1') {
                cout << "Invalid character: only 0 and 1 allowed.\n";
                return false;
            }
            transition(ch);
        }

        cout << "Final State: " << state << endl;
        return state == "q1"; 
    }
};

int main() {
    not_even1 dfa;
    string input;
    cout << "Enter string: ";
        cin >> input;
        if (dfa.run(input))
            cout << "Accepted"<<endl;
        else
            cout << "Rejected"<<endl;
return 0;
}

