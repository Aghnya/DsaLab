#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
   char *a;
   int top;
   int ms;
} ST;

void init(ST *p, int n) {
   p->ms = n;
   p->top = -1;
   p->a = (char*)malloc(sizeof(char) * n);
}

void push(ST *p, char item) {
   if(p->top + 1 >= p->ms) {
        printf("\nError: Stack Overflow!\n");
        exit(1);
   }
   (p->top)++;
   p->a[p->top] = item;
}

char pop(ST *p) {
   if(p->top <= -1) {
       return '\0'; 
   }
   char b = p->a[p->top];
   p->top--;
   return b;
}

char peek(ST *p) {
   if(p->top <= -1) return '\0';
   return p->a[p->top];
}

int isOperand(char ch) {
    return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

int precedence(char ch) {
    if (ch == '^') return 3;
    if (ch == '*' || ch == '/') return 2;
    if (ch == '+' || ch == '-') return 1;
    return 0;
}

void convertInfixToPostfix() {
    char infix[100];
    char postfix[100];
    
    printf("Enter Infix expression: ");
    scanf("%s", infix);

    int len = 0;
    while(infix[len] != '\0') len++;

    ST s;
    init(&s, len + 1);

    int pIdx = 0;           
    int expectOperand = 1;  

    for(int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        if(isOperand(ch)) {
            if (!expectOperand) {
                printf("\nError: Invalid expression! Missing operator between operands.\n");
                exit(1);
            }
            postfix[pIdx++] = ch;
            expectOperand = 0; 
        }
        else if(ch == '(') {
            push(&s, ch);
        }
        else if(ch == ')') {
            if (expectOperand) {
                printf("\nError: Invalid expression! Unexpected closing parenthesis.\n");
                exit(1);
            }
            
            char temp = pop(&s);
            while(temp != '\0' && temp != '(') {
                postfix[pIdx++] = temp;
                temp = pop(&s);
            }
            
            if(temp == '\0') {
                printf("\nError: Invalid expression! Mismatched parentheses (too many right brackets).\n");
                exit(1);
            }
        }
        else if(isOperator(ch)) {
            if (expectOperand) {
                printf("\nError: Invalid expression! Misplaced operator '%c'.\n", ch);
                exit(1);
            }
            
            while(s.top != -1 && precedence(peek(&s)) >= precedence(ch)) {
                postfix[pIdx++] = pop(&s);
            }
            push(&s, ch);
            expectOperand = 1; 
        }
        else {
            printf("\nError: Invalid character '%c' encountered.\n", ch);
            exit(1);
        }
    }

    if (expectOperand) {
        printf("\nError: Invalid expression! Cannot end with an operator.\n");
        exit(1);
    }

    while(s.top != -1) {
        char temp = pop(&s);
        if(temp == '(') {
            printf("\nError: Invalid expression! Mismatched parentheses (too many left brackets).\n");
            exit(1);
        }
        postfix[pIdx++] = temp;
    }

    postfix[pIdx] = '\0'; 
    printf("Corresponding Postfix expression: %s\n", postfix);
    
    free(s.a); 
}

int main() {
    convertInfixToPostfix();
    return 0;
}
