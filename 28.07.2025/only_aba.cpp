#include <iostream>
using namespace std;

class Machine {
    string state = "q0"; 

public:
    void reset() { state = "q0"; }

    string transition(char input) {
        string from = state;
        if (state == "q0") state = (input == 'a') ? "q1" : "qDead";
        else if (state == "q1") state = (input == 'b') ? "q2" : "qDead";
        else if (state == "q2") state = (input == 'a') ? "q3" : "qDead";
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
        return (state == "q3" && input.size() == 3);
    }
};

int main() {
    Machine dfa;
    string input;
    cout << "Enter input: ";
    cin >> input;
    if (dfa.run(input))
        cout << "Accepted"<<endl;
        else
        cout<<"Rejected"<<endl;

    return 0;
}

