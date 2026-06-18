#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
   int *a;
   int top;
   int ms;
} ST;

void init(ST *p, int n) {
   p->ms = n;
   p->top = -1;
   p->a = (int*)malloc(sizeof(int) * n);
}

void push(ST *p, int item) {
   if(p->top + 1 >= p->ms) {
        printf("\nError: Stack Overflow!\n");
        exit(1);
   }
   (p->top)++;
   p->a[p->top] = item;
}

int pop(ST *p) {
   if(p->top <= -1) {
       printf("\nError: Invalid postfix expression! Too many operators.\n");
       exit(1);
   }
   int b = p->a[p->top];
   p->top--;
   return b;
}

int isOperand(char ch) {
    return (ch >= '0' && ch <= '9');
}

int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

void evalPostfix() {
    char post[100];
    printf("Enter Postfix expression: ");
    scanf("%s", post);

    int len = 0;
    while(post[len] != '\0') len++;

    ST s;
    init(&s, len + 1);

    for(int i = 0; post[i] != '\0'; i++) {
        char ch = post[i];

        if(isOperand(ch)) {
            push(&s, ch - '0');
        }
        else if(isOperator(ch)) {
            if(s.top < 1) {
                printf("\nError: Invalid postfix expression! Not enough operands for operator '%c'.\n", ch);
                exit(1);
            }
            int x = pop(&s);
            int y = pop(&s);
            int z;

            switch(ch) {
                case '+': z = y + x; break;
                case '-': z = y - x; break;
                case '*': z = y * x; break;
                case '/': 
                    if(x == 0) {
                        printf("\nError: Division by zero!\n");
                        exit(1);
                    }
                    z = y / x; 
                    break;
            }
            push(&s, z);
        }
        else {
            printf("\nError: Invalid character '%c' encountered.\n", ch);
            exit(1);
        }
    }

    int res = pop(&s);

    if(s.top != -1) {
        printf("\nError: Invalid postfix expression! Too many operands left over.\n");
        exit(1);
    }

    printf("Evaluated value = %d\n", res);
    free(s.a);
}

int main() {
    evalPostfix();
    return 0;
}
