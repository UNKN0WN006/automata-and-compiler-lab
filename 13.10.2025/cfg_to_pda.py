def reverse_push(s):
    return list(s)[::-1] if s != '$' else []

class PDA:
    def __init__(self, terminals, productions, start_symbol='S'):
        self.terminals = terminals
        self.productions = productions
        self.start_symbol = start_symbol
        self.states = ['q0']
        self.accept_states = ['q0']
        self.stack_alphabet = set(terminals)
        for lhs, rhs_list in productions.items():
            self.stack_alphabet.add(lhs)
            for rhs in rhs_list:
                for sym in rhs:
                    self.stack_alphabet.add(sym)
        self.transitions = {}
        self.build_transitions()

    def build_transitions(self):
        # Epsilon transitions for productions: A -> α
        for lhs, rhs_list in self.productions.items():
            for rhs in rhs_list:
                key = ('q0', '$', lhs)
                val = ('q0', rhs if rhs != '' else '$')
                self.transitions.setdefault(key, []).append(val)
        # Terminal matching transitions: a on input and stack
        for t in self.terminals:
            key = ('q0', t, t)
            val = ('q0', '$')
            self.transitions.setdefault(key, []).append(val)

    def run(self, input_string, debug=True):
        from collections import deque
        MAX_STEPS = 10000
        # Each config: (state, stack, idx, steps)
        queue = deque()
        queue.append(('q0', [self.start_symbol], 0, 0))
        while queue:
            state, stack, idx, steps = queue.popleft()
            if steps > MAX_STEPS:
                print("Aborted: Too many steps (possible infinite loop).")
                return False
            current_input = input_string[idx] if idx < len(input_string) else '$'
            top_stack = stack[-1] if stack else '$'
            if debug:
                print(f"Step {steps}: State={state}, Input={input_string[idx:] if idx < len(input_string) else '$'}, Stack={stack}")
            # Accept if stack is empty and input is consumed
            if not stack and idx == len(input_string) and state in self.accept_states:
                print("Accepted!")
                return True
            # Try input transitions (all possible)
            key = (state, current_input, top_stack)
            if key in self.transitions and top_stack != '$':
                for next_state, stack_op in self.transitions[key]:
                    new_stack = stack[:-1] + reverse_push(stack_op)
                    new_idx = idx + (1 if current_input != '$' else 0)
                    queue.append((next_state, new_stack, new_idx, steps+1))
            # Try epsilon transitions (all possible)
            key = (state, '$', top_stack)
            if key in self.transitions and top_stack != '$':
                for next_state, stack_op in self.transitions[key]:
                    new_stack = stack[:-1] + reverse_push(stack_op)
                    queue.append((next_state, new_stack, idx, steps+1))
        print("Rejected!")
        print("Reason: No more transitions possible from the following configuration.")
        print(f"State: {state}")
        print(f"Stack: {stack}")
        print(f"Input remaining: {input_string[idx:] if idx < len(input_string) else '$'}")
        if stack:
            print(f"Top of stack: {stack[-1]}")
        else:
            print("Stack is empty.")
        if idx < len(input_string):
            print(f"Next input symbol: {input_string[idx]}")
        else:
            print("Input is fully consumed.")
        print("If the input should be accepted, check your grammar and transitions for possible missing or incorrect rules.")
        return False

def main():
    print("Enter number of terminal symbols:")
    n = int(input())
    print("Enter terminal symbols separated by space:")
    terminals = input().split()
    print("Enter number of productions:")
    p = int(input())
    print("Enter productions (e.g., S -> aS or S -> b or S -> ε):")
    productions = {}
    for _ in range(p):
        line = input().replace(' ', '')
        lhs, rhs = line.split('->')
        rhs = rhs.replace('ε', '')  # epsilon as empty string
        productions.setdefault(lhs, []).append(rhs)
    print("Enter input string to test:")
    input_string = input()
    pda = PDA(terminals, productions)
    print("\n--- PDA Simulation ---")
    pda.run(input_string, debug=True)

if __name__ == "__main__":
    main()