# Explanations for `first_follow.py` and `parsing.py` (03.11.25)

This document explains two Python programs that compute FIRST and FOLLOW sets for context-free grammars: `first_follow.py` (a compact stdin-driven script) and `parsing.py` (interactive prompt-driven script). It includes purpose, high-level ideas, step-by-step execution flow, key implementation details, why certain Python idioms are used, complexity, limitations, and viva tips.

---

**Question (from `question.txt`)**
- "Write a program in automata and compiler to implement follow and first parsing under compiler theory"

Both scripts implement the standard fixed-point algorithms for FIRST and FOLLOW sets used in compiler construction and parsing table generation.

---

## Shared theory (short)
- FIRST(X): the set of terminals that can begin strings derived from symbol X. If X can derive ε, then ε (or a chosen symbol like '$' or 'ε') belongs to FIRST(X).
- FOLLOW(A): the set of terminals that can appear immediately to the right of nonterminal A in some sentential form; for start symbol, $ is in FOLLOW(start).
- Both sets are computed iteratively until no more elements can be added (a fixed-point iteration). These are textbook algorithms used when building LL(1) parsers and parsing tables.

---

## `first_follow.py` (stdin-driven compact version)

Purpose
- Read a start symbol and number of productions from stdin, parse productions, compute FIRST and FOLLOW sets, and print them.

Key points / flow
1. Read start symbol (first line) and number of productions `n` (second line).
2. Read `n` production lines of the form `A -> α` (space-separated tokens expected on RHS). The script treats several epsilon notations (`ε`, `e`, `$`, `eps`, `epsilon`) as `$` internally.
3. Build `P` as a dict mapping nonterminals to lists of RHS token sequences. Collect nonterminals `NT` by checking token capitalization (an uppercase initial letter indicates a nonterminal in this script's convention).
4. Initialize FIRST[A] = empty set for each nonterminal A.
5. Iteratively compute FIRST sets:
   - For each production A -> X1 X2 ... Xk, traverse the RHS in order. For each symbol Xi
     - If Xi is nonterminal, add FIRST(Xi) excluding '$' to FIRST(A). If FIRST(Xi) contains '$', continue to next Xi; otherwise stop.
     - If Xi is terminal, add Xi to FIRST(A) and stop.
     - If all Xi derive '$' (or the RHS is epsilon), add '$' to FIRST(A).
6. Initialize FOLLOW with FOLLOW[start] contains `$` (end-of-input sentinel).
7. Compute FOLLOW by iterating productions and using a helper `first_seq(seq)` to compute FIRST of a sequence beta (X_{i+1}...Xk), then add FIRST(beta)\{'$'} to FOLLOW of Xi; if FIRST(beta) contains '$' or beta is empty, add FOLLOW(A) to FOLLOW(Xi).
8. Print FIRST and FOLLOW sets sorted.

Notable implementation details
- `isNT(x)` identifies nonterminals by checking `x[0].isupper()` and `x != '$'`. This convention is concise but requires grammar tokens to follow the capitalization rule.
- Epsilon is normalized to `$` for internal consistency.
- The algorithm uses boolean `changed` loops to iterate until sets stabilize.

Why this style?
- Compact and robust for small grammars and educational use. Reading from stdin makes it easy to pipe grammar definitions for batch testing.

Limitations
- The script expects tokens (RHS) to be space-separated; multi-char terminals or combined tokens require pre-tokenization.
- Nonterminals must start with uppercase letters. Terminals are assumed lowercase or non-uppercase.

Viva tips
- Explain you implemented fixed-point iteration for FIRST and FOLLOW; show how FIRST of sequences is computed and how FOLLOW propagates via FIRST(beta) and FOLLOW(A) when beta can derive ε.

---

## `parsing.py` (interactive prompt-driven version)

Purpose
- Prompt the user for number of productions and productions themselves (RHS alternatives separated by `|`), compute FIRST and FOLLOW sets, and print them. This variant uses slightly different conventions (e.g., uses `'ε'` to denote epsilon) and stores productions compactly as strings rather than token lists.

Key points / flow
1. Read number of productions and then each production as `LHS -> RHS1 | RHS2 | ...` where spaces are removed for convenience.
2. Build `p` dict where `p[LHS]` is a list of RHS strings. Collect nonterminals by scanning RHS characters: any uppercase letter is considered a nonterminal.
3. Initialize FIRST and FOLLOW as sets per nonterminal; start symbol is first key read.
4. Compute FIRST sets iteratively:
   - For each RHS, handle explicit epsilons `ε` or empty strings by adding `ε` to FIRST(LHS).
   - Otherwise scan each symbol in RHS left-to-right. If symbol is nonterminal, add FIRST(symbol)-{ε} to FIRST(LHS); if symbol can produce ε, continue, else stop. If symbol is terminal, add it and stop. If all symbols nullable, add ε.
5. Compute FOLLOW sets iteratively similarly to `first_follow.py` but operating on string-encoded RHS.
6. Print FIRST and FOLLOW for each nonterminal.

Notable implementation details
- This script treats each character as a symbol (so multi-character terminals/nonterminals are not directly supported). It uses the `.isupper()` predicate to detect nonterminals.
- It uses `'ε'` as epsilon symbol and also the empty string `''` in places.
- The algorithm structure mirrors textbook fixed-point computations.

Why this design/syntax?
- Simpler input form for interactive demonstration: users can enter productions with `|` alternatives.
- Using sets and Python set operations (`|=`) makes union operations concise and efficient.

Limitations
- Symbol granularity is single-character. For grammars with multi-character tokens (like `id` or `expr`), you would pre-tokenize RHS into tokens.
- The script assumes no ambiguous tokenization and that RHS alternatives are given as contiguous characters (e.g., `aA|b` not `a A | b`).

Viva tips
- Show that both scripts implement the same fixed-point logic with minor differences in input format and epsilon conventions.
- If asked about correctness, explain convergence: sets only grow and are finite (bounded by finite terminal set), so repeated additions terminate.

---

## Common implementation notes and Python idioms used
- Use of `set()` to store FIRST/FOLLOW sets: natural for union operations and membership checks; operations like `|=` and `- {'ε'}` are concise and efficient.
- Fixed-point iteration with a `changed` boolean is the standard pattern for these algorithms.
- Using simple predicates like `sym.isupper()` to detect nonterminals: convenient for textbook-style grammars where nonterminals are uppercase and terminals lowercase.

## Complexity
- Each iteration scans all productions; each set insertion is O(1) average. Worst-case iterations until all possible terminals/nonterminals are added; overall complexity is polynomial in the size of grammar (number of productions × length of RHS × alphabet size).

## Example usage (quick demo)
- For a grammar with productions:
  S -> A B
  A -> a | ε
  B -> b
  The scripts will compute FIRST and FOLLOW sets and print them; in the viva show a small example and trace how FIRST(A) = {a, ε}, FIRST(S) includes a and, if A can be ε, FIRST(B) etc.

---

If you want, I can:
- Convert either script to accept multi-character tokens by adding a tokenizer and updating symbol checks.
- Add unit tests and a small set of example grammars with expected FIRST/FOLLOW outputs.
- Produce a one-page cheat-sheet summarizing what to say in the viva.

Which would you like next?