#include <iostream>
using namespace std;

class even_one 
{
    string state = "q0"; 

public:
    void reset() { state = "q0"; }

    string transition(char input) 
{
        string from = state;
        if (state == "q0") state = (input == '1') ? "q1" : "q0";
        else if (state == "q1") state = (input == '1') ? "q0" : "q1";
        cout << "δ(" << from << ", " << input << ") → " << state << endl;
        return state;
    }

    bool run(const string& input) 
{
        reset();
        cout << "\nInitial State: q0\n";
        for (char ch : input) 
{
            if (ch != '0' && ch != '1') 
{
                cout << "Invalid character '" << ch << "'. Only 0s and 1s allowed."<<endl;
                return false;
            }
            transition(ch);
        }
        cout << "Final State: " << state << endl;
        return (state == "q0");
    }
};

int main() {
    even_one dfa;
    string input;
    cout << "Enter input : "<<endl;
    cin >> input;
    if (dfa.run(input))
    cout << "Accepted"<<endl;
    else
    cout << "Rejected"<<endl;
    return 0;
}

