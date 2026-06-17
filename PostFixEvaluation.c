#include <stdio.h>
#include <stdlib.h>

typedef struct stack{
   int  *a;     // Changed from char* to int* to handle actual evaluation results
   int top;
   int ms;
}ST;

void init(ST *p){
   int n;
   printf("Enter the size of stack: ");
   scanf("%d",&n);
   p->ms=n;
   p->top=-1;
   p->a=(int*)malloc(sizeof(int)*n); // Changed to int allocation
}

void push(ST *p, int item){
   if(p->top+1 >= p->ms){
        printf("\nError: Stack overflow (Expression too long for stack size)!\n");
        exit(1); // Safely exit if stack fills up
   }
   else{
        (p->top)++;
        p->a[p->top]=item;
   }
}

int pop(ST *p){
   if(p->top<=-1){
       // If we try to pop an empty stack, the postfix expression is invalid!
       printf("\nError: Invalid postfix expression (Too many operators)!\n");
       exit(1); 
   }
   else{
      int b=p->a[p->top];
      p->top--;
      return b;
   }
}

int eval(){
    ST s;
    init(&s);
    char post[50];
    printf("Enter post-fix expression: ");
    scanf("%s",post);

    int x, y, z;
    // Removed the double 'int i' declaration
    for(int i=0; post[i]!='\0'; i++){
        if(post[i]>='0' && post[i]<='9'){
             push(&s, post[i]-'0');
        }
        else{
             // Pop the operands
             x=pop(&s); // Right operand
             y=pop(&s); // Left operand
             
             switch(post[i]){
                case '+':
                        z=y+x;
                        push(&s,z);
                        break;
                case '-':
                        z=y-x;
                        push(&s,z);
                        break;
                case '*':
                        z=y*x;
                        push(&s,z);
                        break;
                case '/':
                        if(x == 0) {
                            printf("\nError: Division by zero!\n");
                            exit(1);
                        }
                        z=y/x; // Fixed order: y / x
                        push(&s,z);
                        break;
                default:
                        printf("\nError: Invalid operator '%c' encountered!\n", post[i]);
                        exit(1);
           }
        }
    }
    
    int final_result = pop(&s);
    
    // Edge case: If the stack isn't empty now, they provided too many numbers
    if(s.top != -1) {
        printf("\nError: Invalid postfix expression (Too many operands)!\n");
        exit(1);
    }
    
    // Free the dynamically allocated memory
    free(s.a);
    
    return final_result;
}

int main(){
   printf("Evaluated value = %d\n", eval());
   return 0;
}
