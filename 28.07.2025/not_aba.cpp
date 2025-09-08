#include <iostream>
using namespace std;

class not_aba {
    string state = "q0";

public:
    void reset() { state = "q0"; }

    string transition(char input) {
        string from = state;

        if (state == "q0")
            state = (input == 'a') ? "q1" : "q0";
        else if (state == "q1")
            state = (input == 'b') ? "q2" : "q1";
        else if (state == "q2")
            state = (input == 'a') ? "q3" : "q0";
        else if (state == "q3")
            state = "q3"; 

        cout << "(" << from << ", " << input << ") → " << state << endl;
        return state;
    }

    bool run(const string& input) {
        reset();
        cout << "Initial State: q0"<<endl;

        for (char ch : input) {
            if (ch != 'a' && ch != 'b') {
                cout << "Invalid Output"<<endl;
                return false;
            }
            transition(ch);
        }

        cout << "Final State: " << state << endl;
        return state != "q3";
    }
};

int main() {
    not_aba dfa;
    string input;
    cout << "Enter string: ";
    cin >> input;
    if (dfa.run(input))
    cout << "Accepted"<<endl;
    else
    cout << "Rejected"<<endl;
    return 0;
}	
