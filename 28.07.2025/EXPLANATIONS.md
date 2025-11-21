# Explanations for programs in 28.07.2025

This file explains how each program works, the logic behind the code, the flow of execution, sample I/O, and short tips you can use in your viva. Files covered:
- `2_mul.cpp`
- `even_one.cpp`
- `only_aba.cpp`
- `not_aba.cpp`
- `not_even1.cpp`

---

**`2_mul.cpp` — Recognize binary numbers that are multiples of 2**

- Purpose: Determine whether a given binary string represents a number that is a multiple of 2.
- Key idea: A binary number is a multiple of 2 iff it ends with 0 (and is non-empty). The DFA implemented tracks the last symbol seen and accepts only if final state indicates last symbol was 0.

Flow & logic (simple step-by-step):
- Initialize DFA state to `q0` (represents "last symbol seen = 0").
- For each character in the input:
  - If input is `0`, go to `q0`.
  - If input is `1`, go to `q1`.
  - Any other character → go to `qDead` (invalid).
  - After each transition, it prints `δ(from, input) -> new_state`.
- After processing input, it prints final state and returns accepted only if state is `q0` and input is not empty.

Key code pieces:
- `transition(char input)` updates `state` to `q0`, `q1`, or `qDead` and prints the transition.
- `run(const string& input)` iterates through characters and calls `transition`.
- `main()` reads input and prints "Accepted: multiple of 2" if `run` returns true.

Example:
- Input: `110` → transitions: q0→q1→q1→q0, Final state `q0` → Accepted.
- Input: `` (empty) → Rejected because of `!input.empty()` check.

Complexity: O(n) time, O(1) extra space.

Viva tips (how to explain succinctly):
- Explain that DFA encodes the property "last bit is 0".
- Show that only the last symbol matters; hence only two meaningful states plus dead state.
- Mention the extra check for empty input (the code rejects empty strings).

How to run:
- Compile: `g++ 2_mul.cpp -o 2_mul`
- Run: `./2_mul` and enter a binary string.

---

**`even_one.cpp` — Accept strings with an even number of `1`s**

- Purpose: Accept binary strings that contain an even number of `1` characters.
- Key idea: Use two states: `q0` = even count of `1`s so far (including zero), `q1` = odd count of `1`s so far. Each `1` toggles the state; `0` leaves it unchanged.

Flow & logic:
- Start in `q0`.
- For each character:
  - If character not `0` or `1` → invalid input, return false.
  - If current state `q0`, on `1` go to `q1`, on `0` stay `q0`.
  - If current state `q1`, on `1` go to `q0`, on `0` stay `q1`.
  - Print each transition.
- Accept if final state is `q0` (even number of `1`s).

Key code pieces:
- The transition logic uses a conditional operator to toggle between states.
- `run` validates characters and uses `transition` for each.

Example:
- Input `1010` → `1` toggles to `q1`, `0` stays `q1`, `1` toggles to `q0`, `0` stays `q0` → Final `q0` → Accepted.

Complexity: O(n) time.

Viva tips:
- Explain parity tracking: only need two states because parity is preserved by toggling on `1`.
- Mention input validation: the code rejects non-binary characters.

How to run:
- Compile: `g++ even_one.cpp -o even_one`
- Run: `./even_one` and input a binary string.

---

**`only_aba.cpp` — Accept exactly the string `aba` (length 3 and exact sequence)**

- Purpose: Accept only if the input is exactly the 3-letter string `aba`.
- Key idea: A 4-state DFA that moves through `q0` → `q1` → `q2` → `q3` on `a`, `b`, `a` respectively and anything else goes to `qDead`.

Flow & logic:
- Begin at `q0`.
- Move to `q1` only if first char is `a`, otherwise `qDead`.
- From `q1` move to `q2` only if char is `b`, else `qDead`.
- From `q2` move to `q3` only if char is `a`, else `qDead`.
- After full traversal, the program accepts only if `state == q3` and `input.size() == 3`.

Key code pieces:
- `transition` explicitly checks expected character for each state; else sets `qDead`.
- `run` additionally enforces length 3 (so inputs like `ababa` ending in `aba` are rejected).

Example:
- Input `aba` → transitions to `q3`, length 3 → Accepted.
- Input `aba ` (longer) or `abaX` → Rejected because length != 3 or wrong symbol.

Viva tips:
- Emphasize exact-match DFA; it enforces both order and length.
- Explain the role of `qDead` and the extra size check.

How to run:
- Compile: `g++ only_aba.cpp -o only_aba`
- Run: `./only_aba` and enter `aba` to see Accepted.

---

**`not_aba.cpp` — Accept all strings over `{a,b}` that do NOT contain substring `aba`**

- Purpose: Accept strings that do not contain the substring `aba` anywhere.
- Key idea: Build a DFA that detects `aba` and goes to a rejecting sink state `q3` if the substring appears; otherwise remain in non-rejecting states.

States intuition:
- `q0`: start state / haven't matched any prefix of `aba` yet (or just reset).
- `q1`: saw `a` (prefix `a`).
- `q2`: saw `ab` (prefix `ab`).
- `q3`: saw `aba` at some point — sink rejecting state.

Flow & logic:
- At `q0`:
  - On `a` → `q1` (seen prefix `a`).
  - On `b` → stay `q0`.
- At `q1`:
  - On `b` → `q2` (prefix `ab`).
  - On `a` → stay `q1` (a could be start of new `a`).
- At `q2`:
  - On `a` → `q3` (we have `aba`, enter rejecting sink).
  - On `b` → `q0` (the `ab` followed by `b` breaks the `aba` prefix; no partial match remains).
- At `q3`: stay `q3` for any input (once `aba` occurs, always reject).
- The program accepts iff final state != `q3`.

Key code pieces:
- `transition` explicitly models above rules and prints transitions.
- `run` validates characters and rejects invalid characters.

Examples:
- Input `abba` → transitions avoid completing `aba` → Accepted.
- Input `xaba` (with `x` not `a`/`b`) → the program prints `Invalid Output` and returns false.
- Input `aba` → reaches `q3` → Rejected.

Complexity: O(n).

Viva tips:
- Walk through an example showing how partial matches (`q1`, `q2`) let the DFA detect `aba`.
- Explain why `q3` is a sink reject state.
- If asked for proof: show that any time you reach `q2` and input is `a`, you must have seen `aba`.

How to run:
- Compile: `g++ not_aba.cpp -o not_aba`
- Run: `./not_aba` and enter a string of `a` and `b`.

---

**`not_even1.cpp` — Accept strings with an odd number of `1`s**

- Purpose: Accept binary strings that have an odd number of `1`s (i.e., the complement of `even_one.cpp`).
- Key idea: Use two states `q0` (even number of `1`s) and `q1` (odd). Accept when final state is `q1`.

Flow & logic:
- Start at `q0`.
- On reading `1`, toggle between `q0` and `q1`.
- On `0`, remain in same state.
- Validate input contains only `0` and `1`.
- Accept if final state is `q1`.

Key code pieces:
- `transition` toggles state exactly like `even_one.cpp` but acceptance condition is reversed.
- `run` validates input and prints transitions.

Example:
- Input `1` → `q0`→`q1` → Accepted.
- Input `101` → toggles 3 times → `q1` → Accepted.
- Input `10` → toggles to `q1` then stays → Final `q1` → Accepted.

Viva tips:
- Explain parity argument; demonstrate that tracking parity needs only two states.
- Contrast with `even_one.cpp`: difference is acceptance condition.

How to run:
- Compile: `g++ not_even1.cpp -o not_even1`
- Run: `./not_even1` and input a binary string.

---

**General tips for the viva (short & ready-to-say points):**
- State the property the DFA checks in one sentence (e.g., "Accepts binary strings with an even number of ones").
- Explain the minimum number of states needed and why (parity → 2 states, exact 3-letter match → 4 states + dead state, substring detection → states correspond to matched prefix length).
- When asked about complexity: all programs scan the input once → O(n) time, O(1) memory for state.
- If asked about invalid input: point out input validation in `even_one`, `not_even1`, `not_aba` and `2_mul`/`only_aba` behavior.
- If asked about edge cases: discuss empty string handling (e.g., `2_mul` rejects empty, some others accept/reject depending on property).

---

If you want, I can:
- Split this into separate markdown files per program.
- Add annotated code snippets inline in each section for a line-by-line walkthrough.
- Run each program with example inputs and show the terminal outputs.

Would you like separate per-file markdowns or is this combined document good for your viva notes?