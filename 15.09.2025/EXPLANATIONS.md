# Explanations for 15.09.2025

This document explains the task in `question.txt` and the three implementations provided to analyze a C program: `analyze_c.c` (C implementation), `analyze_c.py` (Python implementation), and `question.c` (sample input C program). It describes the problem, the approach taken by each program, step-by-step flow, key functions/syntax used (including uncommon items), sample output, complexity, limitations, and viva tips.

---

**Question (from `question.txt`)**

Task: Input any C program and:
- Count the number of tokens (excluding whitespaces).
- Build a symbol table with columns: Identifier, Separator, Keyword, Operator, Constant, Special Character.
- Detect lexical errors (illegal characters, overly long identifiers/constants, unmatched string literals, etc.).

This repository includes two analyzers that implement this: `analyze_c.c` (a C program implementing a tokenizer/lexical analyzer) and `analyze_c.py` (a Python script using regular expressions).

---

**`question.c` — Sample input program**

A very small example C program used as input for the analyzers. It defines `main`, declares two integers, computes their sum, prints it, and returns 0. This file is intentionally simple to demonstrate token detection and symbol table generation.

Source (summary):
- `int a = 5, b = 10;` — identifiers, constants, separators, operators, and keywords present.
- `printf("Sum is %d\n", sum);` — function call, string literal (constant), and separator/keywords.

Use this file to test both analyzers.

---

**`analyze_c.c` — C implementation of a lexical analyzer and symbol table builder**

Purpose
- Read a C source file and perform lexical analysis: categorize tokens into identifiers, keywords, separators, operators, constants, and special characters; count tokens; and record lexical errors.

High-level approach
- Low-level character-by-character scanner using `fgetc`.
- State-based branching to handle identifiers/keywords, numeric constants, string/char literals, separators, operators (including multi-character operators like `==`, `!=`, `++`, `--`), preprocessor tokens (starting with `#`), and special characters.
- Symbol table is collected into a `symtab_t` structure, which stores arrays for each category and counts.
- A simple set of lexical checks: max identifier length, max constant length, unmatched quotes, illegal characters, etc. Lexical errors are recorded in an array and printed at the end.

Step-by-step flow / notable code parts
1. Initialization: defines constants (`MAX_LEN`, token buffers), `symtab_t` holding arrays for token categories, `keywords[]` array and helper functions.
2. File input: prompts user for a filename and opens it with `fopen`.
3. Character loop: main loop reads `ch = fgetc(fp)` until EOF.
   - Illegal character check via `isLegalChar` (accepts printable ASCII and whitespace).
   - Whitespace handling: skips spaces/tabs/newlines (they're not counted as tokens).
   - Preprocessor `#`: when `#` is found it collects following alphabetic characters (likely `include`) and handles them specially, saving `#` as a special character and `include` as keyword.
   - Identifiers/keywords: when `isalpha(ch) || ch == '_'` starts, it collects alphanumeric/underscore characters, checks length, then classifies as keyword (using `isKeyword`) or identifier, adding to symbol table.
   - Numeric constants: collects digits and optionally `.` for real-like constants, checks for multiple dots, length limits, and records them.
   - String/char literals: collects until matching quote; if no match, records unmatched-literal lexical error.
   - Separators: checks `,;(){}[]` and adds them to separators table.
   - Operators: handles single-char operators and checks the next character for multi-char operators (`==`, `!=`, `<=`, `>=`, `++`, `--`, `&&`, `||`); uses `ungetc` to push back characters when necessary.
   - Unknown characters produce lexical errors.
4. After scanning, it prints total token count, the symbol table in tabular form, and any lexical errors.

Why some functions/syntax are used (rationale & uncommon items)
- `fgetc`, `ungetc`: low-level file reading giving control over pushing back a character (used after peeking the next character while parsing identifiers, numbers, or operators).
- `isalpha`, `isalnum`, `isdigit`, `isspace`: standard `ctype.h` helpers that simplify classifying characters.
- Arrays of fixed-size strings in `symtab_t`: a simple approach to store tokens without dynamic memory allocation. It’s straightforward but limited in capacity and less flexible than dynamic containers.
- Multi-char operator handling: reads one char, peeks at the next using `fgetc`, and `ungetc` if it doesn't combine. This manual technique is typical in hand-written lexers.
- Error reporting using `snprintf` into `lex_errors`: a simple array-of-strings approach to collect multiple error messages for later printing.

Limitations & caveats (good viva points)
- Fixed-size arrays (`MAX_LEN`, `MAX_TOKENS`) may overflow for large inputs — mention this and suggest dynamic structures (e.g., linked lists or dynamic allocations) as improvements.
- The implementation assumes ASCII printable characters; Unicode/extended characters aren't supported.
- The `isLegalChar` implementation allows many printable ASCII characters; depending on strictness you may want to restrict more.
- Handling of preprocessor directives is minimal (only recognizes alphabetic words after `#`), and it does not parse `#include <...>` fully (it reads alphabetic token after `#`).
- No line/column tracking for error reporting (would be useful for better diagnostics).

Complexity
- Single-pass over file: O(n) where n is number of characters. Tokenization and simple checks are linear-time.

Sample run (using `question.c`):
- Total tokens (excluding whitespace): shows a small integer (depends on exact classification), symbol table prints identifiers `a`, `b`, `sum`, `printf`, constants `5`, `10`, string literal, separators like `;`, `(`, `)`, `{`, `}` and operators `=` `+`, etc.

Improvements you can mention in viva
- Use dynamic arrays (malloc/realloc) or linked structures to avoid fixed upper bounds.
- Maintain line and column counters to report the exact location of lexical errors.
- Support comments more robustly (the current `isLegalChar` rejects no-comment content; comments are not fully handled in this C lexer), and support full preprocessor token parsing (including `<...>` includes).

---

**`analyze_c.py` — Python implementation using regular expressions**

Purpose
- Provide a concise, readable analyzer that tokenizes the C input using regexes and categorizes tokens into the same symbol-table columns, plus reports lexical errors.

High-level approach
- Read the file into a string; remove comments with regex; then use `re.findall` with alternation over identifier, constant, operator, separator, special, or string literal patterns.
- For each token, use regex fullmatches and membership tests to categorize tokens into `Identifier`, `Keyword`, `Operator`, `Separator`, `Constant`, or `Special Character` sets.

Step-by-step flow / notable code parts
1. Define sets of `KEYWORDS`, `OPERATORS`, `SEPARATORS`, and `SPECIAL_CHARS`.
2. Regex patterns built for identifiers, constants, operators (including multi-char), separators, and special characters. Tokenization uses an alternation to find all tokens.
3. Remove comments with `re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)` and `re.sub(r'//.*', '', code)` before tokenizing.
4. `tokens = re.findall(...)` returns a list of token strings. The script iterates tokens, increases `token_count`, and classifies each one.
5. Builds a `symbol_table` dictionary of sets and prints a simple textual table.

Why this approach / notable Python features
- Regular expressions: allow compact tokenization, including multi-character operators and string literals.
- `set()` for symbol_table columns ensures uniqueness automatically — simpler than manual deduplication.
- Using `re.fullmatch` to precisely classify tokens is concise and robust for many typical cases.

Limitations & caveats
- Regex-based tokenization can fail in edge cases (e.g., nested quotes, escaped quotes inside strings, malformed code) unless the regex is carefully tuned.
- The script counts tokens found by regex alternation; some tokens may be ambiguous if patterns overlap incorrectly — the order and exact patterns matter.
- The script removes comments with regex which is acceptable here but can fail on edge cases like string literals containing `/*` or `//`. Handling those robustly typically requires a lexer that understands string boundaries before stripping comments.

Sample run (already wired to `question.c`):
- Prints `Total tokens`, then a single-line symbol table showing comma-separated tokens in each category. If no lexical errors, prints "No lexical errors found." Otherwise, prints tokens that didn't match known categories.

Complexity
- Regex search is linear-ish for typical inputs, but pathological regexes can blow up. For usual C programs of moderate size this approach is efficient and compact.

---

**Which implementation to use and why**
- `analyze_c.c` shows how to write a hand-crafted lexer in C: good for learning and for tight control (e.g., handling `ungetc`, peeking, exact char-level logic). It's closer to how a compiler front-end works in low-level details.
- `analyze_c.py` is more practical for quick tasks and demonstrations: fewer lines, easier to maintain, extensive use of Python libraries (regex, sets), and easier to extend for quick experiments.

For your viva: emphasize that both implement the same core idea (tokenization + categorization), but one is manual (C) and one is high-level (Python). Both have trade-offs: C is precise and instructive; Python is concise and fast to iterate on.

---

**How to run these analyzers**

C analyzer:
```bash
# Compile
gcc analyze_c.c -o analyze_c
# Run and provide filename (e.g., "question.c") when prompted
./analyze_c
```

Python analyzer:
```bash
python3 analyze_c.py
# It runs `analyze_c_file("question.c")` by default in current code.
```

**Viva cheat-sheet (one-liners)**
- "We perform lexical analysis by scanning characters and grouping them into tokens: identifiers, constants, keywords, separators, operators, and special characters."
- "In C we implement a manual scanner using `fgetc`/`ungetc`, `ctype.h` helpers, and arrays to store tokens. In Python we use regexes and sets to achieve the same result more concisely."
- "Improvements: dynamic memory for arbitrary length programs, line/column tracking for error reporting, and more robust handling of strings/comments/preprocessor constructs."

---

If you want, I can next:
- Run the C analyzer on `question.c` in this workspace and capture its exact output (requires compiling the C program). I can do the same with the Python script and put results into a `RUN_OUTPUTS` file.
- Add line/column error reporting to `analyze_c.c` and/or replace fixed-size arrays with dynamically allocated structures.
- Make the Python regex more robust to escaped quotes and corner cases.

Which would you like me to do?