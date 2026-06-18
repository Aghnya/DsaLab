#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct stack {
    int top;
    int MS;
    char *a;	
} ST;

// Initializes the stack based on the length of the expression
void init(ST *p, int size) {
    p->MS = size;
    p->top = -1;
    p->a = (char*)malloc(sizeof(char) * p->MS);
}

int overflow(ST *p) {
    return (p->top == p->MS - 1);
}

int underflow(ST *p) {
    return (p->top == -1); 
}

char pop(ST *p) {
    if(underflow(p)) {
        return '\0';  
    }
    return p->a[p->top--]; 
}

void push(ST *p, char n) {
    if(overflow(p)) {
        printf("Stack full\n");
        return; 
    }
    p->a[++p->top] = n;
}

int pr(char ch) {
    if (ch == '$' || ch == '^')
        return 3;
    else if (ch == '*' || ch == '/')
        return 2;
    else if (ch == '+' || ch == '-')
        return 1;
    else
        return 0; 
}

// Validation Function to check if the infix expression is syntactically correct
bool isValidInfix(const char *in) {
    int paren_count = 0;
    bool expect_operand = true; 
    int i = 0;

    while (in[i] != '\0') {
        if (isspace(in[i])) {
            i++;
            continue;
        }

        if (isalnum(in[i])) {
            if (!expect_operand) return false; // Two operands together without operator
            
            // Fast-forward through the multi-digit number or word
            while (isalnum(in[i])) {
                i++;
            }
            expect_operand = false; // Next we expect an operator or ')'
            continue; // Already advanced 'i', so we skip the i++ at the bottom
            
        } else if (in[i] == '(') {
            if (!expect_operand) return false; // e.g., "A(" is invalid
            paren_count++;
            
        } else if (in[i] == ')') {
            if (expect_operand) return false; // e.g., "()" or "+)" is invalid
            paren_count--;
            if (paren_count < 0) return false; // Unbalanced closing parenthesis
            
        } else if (strchr("+-*/^$", in[i])) {
            if (expect_operand) return false; // e.g., starting with "+" or "*+"
            expect_operand = true; // After an operator, we expect an operand
            
        } else {
            return false; // Invalid character found
        }
        i++;
    }
    
    // Valid if parentheses balance and we aren't left expecting an operand (e.g., ending with "+")
    return (paren_count == 0 && !expect_operand);
}

void convert(char in[], char post[]) {
    int i, j = 0;
    char x;  
    ST s;
    
    // Safe initialization based on input length
    init(&s, strlen(in) + 1); 
    
    for(i = 0; in[i] != '\0'; i++) {
        // Ignore spaces in the input
        if (isspace(in[i])) continue; 

        if (isalnum(in[i])) {
            // Read the entire multi-digit number or alphanumeric block
            while (isalnum(in[i])) {
                post[j++] = in[i++]; 
            }
            post[j++] = ' '; // Add space separator for postfix format
            i--; // Decrement because the for-loop will increment i again
        }
        else {
            switch(in[i]) {
                case '(': 
                    push(&s, in[i]); 
                    break;
                case ')': 
                    while(!underflow(&s) && (x = pop(&s)) != '(') {
                        post[j++] = x;
                        post[j++] = ' '; // Add space after operator
                    }
                    break;
                default:  
                    while(!underflow(&s) && pr(in[i]) <= pr(s.a[s.top])) {
                        post[j++] = pop(&s);
                        post[j++] = ' '; // Add space after operator
                    }
                    push(&s, in[i]); 
            }
        }
    }
    
    // Pop any remaining operators from the stack
    while(!underflow(&s)) {
        post[j++] = pop(&s);
        post[j++] = ' ';
    }
    
    // Remove the very last trailing space if it exists
    if (j > 0 && post[j-1] == ' ') {
        j--;
    }
    post[j] = '\0';
    
    printf("The postfix notation = %s\n", post); 
    free(s.a); // Prevent memory leak
}

int main() {
    char in, post; // Increased buffer size to account for spaces
    
    printf("\nEnter the infix notation: ");
    if (fgets(in, sizeof(in), stdin) != NULL) {
        // Strip out the newline character captured by fgets
        in[strcspn(in, "\n")] = 0;

        if (strlen(in) == 0) {
            printf("Error: Empty expression.\n");
            return 1;
        }

        if (isValidInfix(in)) {
            convert(in, post);
        } else {
            printf("Error: Invalid infix expression.\n");
            printf("Check for balanced parentheses, missing operators, or invalid characters.\n");
        }
    }
    
    return 0;
}
