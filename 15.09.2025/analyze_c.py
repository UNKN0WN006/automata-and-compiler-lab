import re

# Define C keywords, operators, separators, and special characters
KEYWORDS = {
    'auto', 'break', 'case', 'char', 'const', 'continue', 'default', 'do', 'double', 'else', 'enum', 'extern',
    'float', 'for', 'goto', 'if', 'int', 'long', 'register', 'return', 'short', 'signed', 'sizeof', 'static',
    'struct', 'switch', 'typedef', 'union', 'unsigned', 'void', 'volatile', 'while', 'printf', 'scanf'
}
OPERATORS = {'+', '-', '*', '/', '%', '++', '--', '=', '==', '!=', '<', '>', '<=', '>=', '&&', '||', '!', '&', '|', '^', '~', '<<', '>>'}
SEPARATORS = {',', ';', '(', ')', '{', '}', '[', ']'}
SPECIAL_CHARS = {'#', '<', '>'}

# Regex patterns
identifier_pattern = r'\b[_a-zA-Z][_a-zA-Z0-9]*\b'
constant_pattern = r'\b\d+\b'
operator_pattern = r'\+\+|--|==|!=|<=|>=|<<|>>|&&|\|\||[+\-*/%=&|^~!<>]'
separator_pattern = r'[;,(){}\[\]]'
special_pattern = r'[<>#]'

symbol_table = {
    'Identifier': set(),
    'Separator': set(),
    'Keyword': set(),
    'Operator': set(),
    'Constant': set(),
    'Special Character': set()
}
lexical_errors = []
token_count = 0

def analyze_c_file(filename):
    global token_count
    with open(filename, 'r') as f:
        code = f.read()

    # Remove comments
    code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)
    code = re.sub(r'//.*', '', code)

    # Tokenize
    tokens = re.findall(rf'{identifier_pattern}|{constant_pattern}|{operator_pattern}|{separator_pattern}|{special_pattern}|".*?"', code)
    for token in tokens:
        if token.isspace():
            continue
        token_count += 1
        if token in KEYWORDS:
            symbol_table['Keyword'].add(token)
        elif re.fullmatch(identifier_pattern, token):
            if token not in KEYWORDS:
                symbol_table['Identifier'].add(token)
        elif re.fullmatch(constant_pattern, token):
            symbol_table['Constant'].add(token)
        elif re.fullmatch(operator_pattern, token):
            symbol_table['Operator'].add(token)
        elif re.fullmatch(separator_pattern, token):
            symbol_table['Separator'].add(token)
        elif re.fullmatch(special_pattern, token):
            symbol_table['Special Character'].add(token)
        elif re.fullmatch(r'".*?"', token):
            symbol_table['Constant'].add(token)
        else:
            lexical_errors.append(token)

    print(f"Total tokens (excluding whitespaces): {token_count}")
    print("\nSymbol Table:")
    print("__________________________________________________________________")
    print("|Identifier|Separator|Keyword|Operator|Constant|Special Character|")
    print("|", end="")
    print(f"{', '.join(symbol_table['Identifier'])}|", end="")
    print(f"{', '.join(symbol_table['Separator'])}|", end="")
    print(f"{', '.join(symbol_table['Keyword'])}|", end="")
    print(f"{', '.join(symbol_table['Operator'])}|", end="")
    print(f"{', '.join(symbol_table['Constant'])}|", end="")
    print(f"{', '.join(symbol_table['Special Character'])}|")
    print("__________________________________________________________________")
    if lexical_errors:
        print("\nLexical Errors:")
        for err in lexical_errors:
            print(err)
    else:
        print("\nNo lexical errors found.")

if __name__ == "__main__":
    analyze_c_file("question.c")
