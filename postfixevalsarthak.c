#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct stack {
    int top;
    int MS;
    int *a;	
} ST;

/* Initialize stack based on string length */
void init(ST *p, int size) {
    p->MS = size;
    p->top = -1;
    p->a = (int*)malloc(sizeof(int) * p->MS);
}

int overflow(ST *p) {
    return (p->top == p->MS - 1);
}

int underflow(ST *p) {
    return (p->top == -1); 
}

int pop(ST *p) {
    if(underflow(p)) {
        return 0;  
    }
    return p->a[p->top--]; 
}

void push(ST *p, int n) {
    if(overflow(p)) {
        printf("Stack full\n");
        return; 
    }
    p->a[++p->top] = n;
}

/* Validates the postfix string */
int isValidPostfix(const char *post) {
    int operand_count = 0;
    int i = 0;

    while (post[i] != '\0') {
        if (isspace(post[i])) {
            i++;
            continue;
        }

        if (isdigit(post[i])) {
            operand_count++;
            /* Skip the rest of the multi-digit number */
            while (isdigit(post[i])) {
                i++;
            }
            continue; 
        } 
        else if (strchr("+-*/^", post[i])) {
            /* An operator needs at least 2 operands available */
            if (operand_count < 2) return 0; 
            
            /* Two operands popped, one result pushed. Net change: -1 */
            operand_count--; 
        } 
        else {
            /* Invalid character found (e.g., letters or parentheses) */
            return 0; 
        }
        i++;
    }
    
    /* A valid postfix expression results in exactly 1 final operand */
    return (operand_count == 1);
}

void post_eval(const char post[]) {
    ST s; 
    int i = 0, x, y, z;
    
    init(&s, strlen(post) + 1);
    
    while(post[i] != '\0') {
        /* Skip spaces */
        if (isspace(post[i])) {
            i++;
            continue;
        }
        
        /* Multi-digit number construction */
        if (isdigit(post[i])) {
            int num = 0;
            while (isdigit(post[i])) {
                num = (num * 10) + (post[i] - '0');
                i++;
            }
            push(&s, num);
            continue; /* i is already advanced past the number */
        }
        else {
            /* Pop the top two values for the operator */
            x = pop(&s); /* x is the second operand */
            y = pop(&s); /* y is the first operand */
            
            switch(post[i]) {
                case '+':    
                    z = y + x; 
                    push(&s, z); 
                    break; 
                case '-':   
                    z = y - x;
                    push(&s, z);
                    break; 
                case '*':   
                    z = y * x; 
                    push(&s, z);
                    break; 
                case '/':  
                    if (x == 0) {
                        printf("Error: Mathematical Division by Zero.\n");
                        free(s.a);
                        return;
                    }
                    z = y / x;
                    push(&s, z);
                    break; 
            }
            i++;
        }
    }
    
    printf("Final value = %d\n", pop(&s)); 
    free(s.a); /* Free allocated memory */
}

int main() {
    char post;		
    
    printf("\nEnter postfix notation: ");
    if (fgets(post, sizeof(post), stdin) != NULL) {
        
        /* Remove newline character from fgets */
        post[strcspn(post, "\n")] = 0;

        if (strlen(post) == 0) {
            printf("Error: Empty expression.\n");
            return 1;
        }

        if (isValidPostfix(post)) {
            post_eval(post);
        } else {
            printf("Error: Invalid postfix expression.\n");
            printf("Ensure operands and operators are correct, and no parentheses exist.\n");
        }
    }
    
    return 0;
}
