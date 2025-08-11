#include <bits/stdc++.h>
using namespace std;

struct DFA {
    int states, symbols;
    vector<vector<int>> trans; // transition table
    vector<bool> isFinal;
};

// Function to check if two states are distinguishable in the current partition
bool areDistinguishable(int s1, int s2, const vector<int> &group, const DFA &dfa) {
    if (dfa.isFinal[s1] != dfa.isFinal[s2]) return true; // final vs non-final

    // For each symbol, see if transitions lead to different groups
    for (int sym = 0; sym < dfa.symbols; sym++) {
        if (group[dfa.trans[s1][sym]] != group[dfa.trans[s2][sym]])
            return true;
    }
    return false;
}

// Check if minimization is required at all
bool isMinimizationNeeded(const DFA &dfa) {
    vector<int> group(dfa.states);
    for (int i = 0; i < dfa.states; i++) group[i] = dfa.isFinal[i] ? 1 : 0;

    // Quick check: see if there is at least one pair in the same group but distinguishable
    for (int i = 0; i < dfa.states; i++) {
        for (int j = i + 1; j < dfa.states; j++) {
            if (group[i] == group[j] && areDistinguishable(i, j, group, dfa)) {
                return true; // At least one split needed
            }
        }
    }
    return false;
}

// Minimize DFA using equivalence theorem
DFA minimizeDFA(const DFA &dfa) {
    vector<int> group(dfa.states);
    for (int i = 0; i < dfa.states; i++) group[i] = dfa.isFinal[i] ? 1 : 0;

    bool changed;
    do {
        changed = false;
        map<vector<int>, int> newGroups; // Map transitions pattern to new group
        vector<int> nextGroup(dfa.states);

        for (int i = 0; i < dfa.states; i++) {
            vector<int> key = {group[i]};
            for (int sym = 0; sym < dfa.symbols; sym++)
                key.push_back(group[dfa.trans[i][sym]]);

            if (newGroups.find(key) == newGroups.end()) {
                int newId = newGroups.size();
                newGroups[key] = newId;
            }
            nextGroup[i] = newGroups[key];
        }

        if (nextGroup != group) {
            group = nextGroup;
            changed = true;
        }
    } while (changed);

    // Build minimized DFA
    int newStates = *max_element(group.begin(), group.end()) + 1;
    DFA minDFA;
    minDFA.states = newStates;
    minDFA.symbols = dfa.symbols;
    minDFA.trans.assign(newStates, vector<int>(dfa.symbols, 0));
    minDFA.isFinal.assign(newStates, false);

    for (int i = 0; i < dfa.states; i++) {
        int newState = group[i];
        for (int sym = 0; sym < dfa.symbols; sym++) {
            minDFA.trans[newState][sym] = group[dfa.trans[i][sym]];
        }
        if (dfa.isFinal[i]) minDFA.isFinal[newState] = true;
    }

    return minDFA;
}

int main() {
    DFA dfa;
    cout << "Enter number of states: ";
    cin >> dfa.states;
    cout << "Enter number of symbols: ";
    cin >> dfa.symbols;

    dfa.trans.assign(dfa.states, vector<int>(dfa.symbols));
    dfa.isFinal.assign(dfa.states, false);

    cout << "Enter transition table (state for each symbol):\n";
    for (int i = 0; i < dfa.states; i++) {
        for (int j = 0; j < dfa.symbols; j++) {
            cin >> dfa.trans[i][j];
        }
    }

    int f;
    cout << "Enter number of final states: ";
    cin >> f;
    cout << "Enter final states: ";
    for (int i = 0; i < f; i++) {
        int st;
        cin >> st;
        dfa.isFinal[st] = true;
    }

    // Step 1: Check if minimization is needed
    if (!isMinimizationNeeded(dfa)) {
        cout << "\nNo minimization needed.\n";
        return 0;
    }

    // Step 2: Minimize DFA
    DFA minDFA = minimizeDFA(dfa);

    // Step 3: Output minimized DFA
    cout << "\nMinimized DFA:\n";
    cout << "Number of states: " << minDFA.states << "\n";
    cout << "Transition table:\n";
    for (int i = 0; i < minDFA.states; i++) {
        for (int j = 0; j < minDFA.symbols; j++) {
            cout << minDFA.trans[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "Final states: ";
    for (int i = 0; i < minDFA.states; i++) {
        if (minDFA.isFinal[i]) cout << i << " ";
    }
    cout << "\n";
    return 0;
}
