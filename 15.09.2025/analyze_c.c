#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100
#define MAX_TOKENS 1000
#define ID_MAX_LEN 31
#define CONST_MAX_LEN 20

// Token categories
typedef struct {
    char id[MAX_LEN][ID_MAX_LEN];
    int id_count;
    char sep[MAX_LEN][2];
    int sep_count;
    char key[MAX_LEN][ID_MAX_LEN];
    int key_count;
    char op[MAX_LEN][3];
    int op_count;
    char con[MAX_LEN][CONST_MAX_LEN];
    int con_count;
    char spc[MAX_LEN][2];
    int spc_count;
} symtab_t;

symtab_t sym;
char *keywords[] = {
    "auto","break","case","char","const","continue","default","do","double","else","enum",
    "extern","float","for","goto","if","int","long","register","return","short","signed",
    "sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while",
    "include","printf","scanf","main"
};
int keysize = sizeof(keywords)/sizeof(char*);

// Lexical error table
char lex_errors[MAX_TOKENS][MAX_LEN];
int lex_err_count = 0;

int isKeyword(char *str) {
    for(int i=0; i<keysize; i++)
        if(strcmp(str, keywords[i]) == 0) return 1;
    return 0;
}

// Check if string already in symbol table array
int exists(char arr[][MAX_LEN], int count, char *str) {
    for(int i=0; i<count; i++)
        if(strcmp(arr[i], str) == 0) return 1;
    return 0;
}


// Ientifiers and keywords
void add_id_or_key(char arr[][ID_MAX_LEN], int *count, char *str) {
    for(int i=0; i<*count; i++)
        if(strcmp(arr[i], str) == 0) return;
    if(*count < MAX_LEN) {
        strncpy(arr[*count], str, ID_MAX_LEN-1);
        arr[*count][ID_MAX_LEN-1] = '\0';
        (*count)++;
    }
}
// Sparators and special characters
void add_sep_or_spc(char arr[][2], int *count, char *str) {
    for(int i=0; i<*count; i++)
        if(strcmp(arr[i], str) == 0) return;
    if(*count < MAX_LEN) {
        strncpy(arr[*count], str, 1);
        arr[*count][1] = '\0';
        (*count)++;
    }
}
// Oerators
void add_op(char arr[][3], int *count, char *str) {
    for(int i=0; i<*count; i++)
        if(strcmp(arr[i], str) == 0) return;
    if(*count < MAX_LEN) {
        strncpy(arr[*count], str, 2);
        arr[*count][2] = '\0';
        (*count)++;
    }
}
// Constants
void add_const(char arr[][CONST_MAX_LEN], int *count, char *str) {
    for(int i=0; i<*count; i++)
        if(strcmp(arr[i], str) == 0) return;
    if(*count < MAX_LEN) {
        strncpy(arr[*count], str, CONST_MAX_LEN-1);
        arr[*count][CONST_MAX_LEN-1] = '\0';
        (*count)++;
    }
}
// Print symbol table header
void print_header() {
    printf("____________________________________________________________________\n");
    printf("| %-10s | %-9s | %-7s | %-8s | %-8s | %-15s |\n",
           "Identifier","Separator","Keyword","Operator","Constant","Special Character");
    printf("--------------------------------------------------------------------\n");
}

void print_symtab(symtab_t *s) {
    int maxr = s->id_count;
    if(s->sep_count > maxr) maxr = s->sep_count;
    if(s->key_count > maxr) maxr = s->key_count;
    if(s->op_count > maxr) maxr = s->op_count;
    if(s->con_count > maxr) maxr = s->con_count;
    if(s->spc_count > maxr) maxr = s->spc_count;

    for(int i=0; i<maxr; i++) {
        printf("| %-10s | %-9s | %-7s | %-8s | %-8s | %-15s |\n",
               i < s->id_count ? s->id[i] : "",
               i < s->sep_count ? s->sep[i] : "",
               i < s->key_count ? s->key[i] : "",
               i < s->op_count ? s->op[i] : "",
               i < s->con_count ? s->con[i] : "",
               i < s->spc_count ? s->spc[i] : "");
    }
    printf("____________________________________________________________________\n");
}

// Printing lexical errors in table format at end
void print_lex_errors() {
    if(lex_err_count == 0) {
        printf("\nNo lexical errors detected.\n");
        return;
    }
    printf("\nLexical Errors:\n");
    printf("---------------------------------------------\n");
    printf("| %-40s |\n", "Error Description");
    printf("---------------------------------------------\n");
    for(int i=0; i<lex_err_count; i++) {
        printf("| %-40s |\n", lex_errors[i]);
    }
    printf("---------------------------------------------\n");
}

// Check if char is separator
int isSeparator(char ch) {
    char seps[] = ",;(){}[]";
    for(int i=0; i<(int)strlen(seps); i++)
        if(ch == seps[i]) return 1;
    return 0;
}

// Check if char is operator (including multi-char handled separately)
int isOperator(char ch) {
    char ops[] = "+-*/%=!<>|&^~";
    for(int i=0; i<(int)strlen(ops); i++)
        if(ch == ops[i]) return 1;
    return 0;
}

// Check if char is special character (# for example)
int isSpecial(char ch) {
    return ch == '#';
}

// Lexical error flags
int lex_err = 0;

// Function to check if identifier length exceeds limit
int isIdLengthValid(char *str) {
    return strlen(str) <= ID_MAX_LEN;
}

// Function to check if constant length exceeds limit
int isConstLengthValid(char *str) {
    return strlen(str) <= CONST_MAX_LEN;
}

// Check illegal chars
int isLegalChar(char ch) {
    // allow ASCII printable chars and tabs/newlines etc
    if(ch == '\n' || ch == '\t' || ch == ' ') return 1;
    if(ch >= 32 && ch <= 126) return 1;
    return 0;
}

int main() {
    char ch, temp[MAX_LEN];
    int i = 0, state = 0, token_count = 0;
    FILE *fp;
    sym.id_count = sym.sep_count = sym.key_count = sym.op_count = sym.con_count = sym.spc_count = 0;
    printf("Enter file name containing C program to analyze: ");
    char fname[50];
    scanf("%s", fname);
    fp = fopen(fname, "r");
    if(fp == NULL) {
        printf("File not found!\n");
        return 1;
    }

    printf("\nLexical Analysis & Symbol Table:\n\n");
    int prev_nonspace = '\0';
    int missing_semicolon_flag = 0;
    while((ch = fgetc(fp)) != EOF) {
        // Check for illegal character
        if(!isLegalChar(ch)) {
            snprintf(lex_errors[lex_err_count++], MAX_LEN, "Illegal character '%c'", ch);
            continue;
        }

        if(ch == ' ' || ch == '\t' || ch == '\n') {
            // whitespace, skip but reset state/token
            continue;
        }

        if(ch == '#') {
            char s[2] = {ch, '\0'};
            add_sep_or_spc(sym.spc, &sym.spc_count, s);
            token_count++;
            int j = 0;
            char inc[8] = {0};
            while((ch = fgetc(fp)) != EOF && isalpha(ch)) {
                if(j < 7) inc[j++] = ch;
            }
            inc[j] = '\0';
            if(strcmp(inc, "include") == 0) {
                add_id_or_key(sym.key, &sym.key_count, inc);
            }
            if(ch == EOF) break;
            ungetc(ch, fp);
            continue;
        }

        // If letter or underscore => identifier/keyword
        if(isalpha(ch) || ch == '_') {
            i = 0;
            temp[i++] = ch;
            while((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
                if(i < MAX_LEN-1) temp[i++] = ch;
                else {
                    snprintf(lex_errors[lex_err_count++], MAX_LEN, "Identifier too long");
                    while((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')); // skip rest of long token
                    break;
                }
            }
            temp[i] = '\0';
            if(isKeyword(temp)) {
                add_id_or_key(sym.key, &sym.key_count, temp);
            } else {
                if(!isIdLengthValid(temp)) {
                    snprintf(lex_errors[lex_err_count++], MAX_LEN, "Identifier exceeds max length: %.30s", temp);
                }
                add_id_or_key(sym.id, &sym.id_count, temp);
            }
            token_count++;
            if(ch == EOF) break;
            ungetc(ch, fp); // push back char for next loop
            prev_nonspace = temp[0];
        }
        // Numeric constant
        else if(isdigit(ch)) {
            i = 0;
            temp[i++] = ch;
            int dotcount = 0;
            while((ch = fgetc(fp)) != EOF && (isdigit(ch) || ch == '.')) {
                if(ch == '.') dotcount++;
                if(dotcount > 1) break;
                if(i < MAX_LEN-1) temp[i++] = ch;
                else {
                    snprintf(lex_errors[lex_err_count++], MAX_LEN, "Numeric constant too long");
                    while((ch = fgetc(fp)) != EOF && (isdigit(ch) || ch == '.'));
                    break;
                }
            }
            temp[i] = '\0';
            if(!isConstLengthValid(temp)) {
                snprintf(lex_errors[lex_err_count++], MAX_LEN, "Constant exceeds max length: %.19s", temp);
            }
            add_const(sym.con, &sym.con_count, temp);
            token_count++;
            if(ch == EOF) break;
            ungetc(ch, fp);
            prev_nonspace = temp[0];
        }
        // Character literal or string literal
        else if(ch == '"' || ch == '\'') {
            char start = ch;
            i = 0;
            temp[i++] = ch;
            int matched = 0;
            while((ch = fgetc(fp)) != EOF) {
                temp[i++] = ch;
                if(ch == start) {
                    matched = 1;
                    break;
                }
                if(i >= MAX_LEN-1) {
                    snprintf(lex_errors[lex_err_count++], MAX_LEN, "String literal too long");
                    break;
                }
            }
            if(!matched) {
                snprintf(lex_errors[lex_err_count++], MAX_LEN, "Unmatched string literal");
            }
            temp[i] = '\0';
            add_const(sym.con, &sym.con_count, temp);
            token_count++;
            prev_nonspace = temp[0];
        }
        // Separator
        else if(isSeparator(ch)) {
            char s[2];
            s[0] = ch;
            s[1] = '\0';
            add_sep_or_spc(sym.sep, &sym.sep_count, s);
            token_count++;
            // Check for missing semicolon (basic): if previous nonspace is '}' or identifier
            if(ch == ';') missing_semicolon_flag = 0;
            else if(ch == '}') missing_semicolon_flag = 1;
            prev_nonspace = ch;
        }
        // Special char
        else if(isSpecial(ch)) {
            char s[2];
            s[0] = ch;
            s[1] = '\0';
            add_sep_or_spc(sym.spc, &sym.spc_count, s);
            token_count++;
            prev_nonspace = ch;
        }
        // Operator (handle multi-char operators)
        else if(isOperator(ch)) {
            char op[3];
            op[0] = ch;
            op[1] = '\0';
            char nxt = fgetc(fp);
            if(nxt != EOF) {
                // check for multi-char operators
                if((ch == '+' && nxt == '+') ||
                   (ch == '-' && nxt == '-') ||
                   (ch == '=' && nxt == '=') ||
                   (ch == '!' && nxt == '=') ||
                   (ch == '<' && nxt == '=') ||
                   (ch == '>' && nxt == '=') ||
                   (ch == '&' && nxt == '&') ||
                   (ch == '|' && nxt == '|')) {
                    op[1] = nxt;
                    op[2] = '\0';
                } else {
                    ungetc(nxt, fp);
                }
            }
            add_op(sym.op, &sym.op_count, op);
            token_count++;
            prev_nonspace = op[0];
        }
        else {
            // Unknown character
            snprintf(lex_errors[lex_err_count++], MAX_LEN, "Unknown character '%c'", ch);
        }
    }

    fclose(fp);

    printf("Total tokens (excluding whitespace): %d\n\n", token_count);
    print_header();
    print_symtab(&sym);
    print_lex_errors();

    return 0;
}
