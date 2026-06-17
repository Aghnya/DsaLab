#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct stack{
    char *a;
    int top;
    int ms;
}ST;

void init(ST *p){
    int n;
    printf("Enter the size of stack: ");
    scanf("%d",&n);

    p->ms=n;
    p->top=-1;
    p->a=(char*)malloc(sizeof(char)*n);

    if(p->a==NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
}

void push(ST *p,char item){
    if(p->top+1 >= p->ms){
        printf("Stack Full\n");
        return;
    }

    p->a[++(p->top)] = item;
}

int pop(ST *p){
    if(p->top==-1){
        return -1;
    }

    return p->a[(p->top)--];
}

int underflow(ST *p){
    return p->top==-1;
}

int isOperator(char c){
    return (c=='+' || c=='-' || c=='*' || c=='/' || c=='^');
}

int precedence(char c){
    if(c=='^')
        return 3;
    else if(c=='*' || c=='/')
        return 2;
    else if(c=='+' || c=='-')
        return 1;
    return 0;
}

void convert(ST *p,char in[],char post[]){

    int n=strlen(in);

    if(n==0){
        printf("Empty Expression\n");
        return;
    }

    /* Expression should not start or end with operator */
    if(isOperator(in[0]) || isOperator(in[n-1])){
        printf("Invalid Expression\n");
        return;
    }

    /* Consecutive operators check */
    for(int i=0;i<n-1;i++){
        if(isOperator(in[i]) && isOperator(in[i+1])){
            printf("Invalid Expression\n");
            return;
        }
    }

    int j=0;

    for(int i=0;i<n;i++){

        char c=in[i];

        if(c==' ' || c=='\t')
            continue;

        if(isalnum(c)){
            post[j++]=c;
        }

        else if(c=='('){
            push(p,c);
        }

        else if(c==')'){

            while(!underflow(p) && p->a[p->top]!='('){
                post[j++]=(char)pop(p);
            }

            if(underflow(p)){
                printf("Invalid Expression\n");
                return;
            }

            pop(p);
        }

        else if(isOperator(c)){

            while(!underflow(p) &&
                  p->a[p->top]!='(' &&
                  precedence(p->a[p->top]) >= precedence(c))
            {
                post[j++]=(char)pop(p);
            }

            push(p,c);
        }

        else{
            printf("Invalid Character Found\n");
            return;
        }
    }

    while(!underflow(p)){

        if(p->a[p->top]=='('){
            printf("Invalid Expression\n");
            return;
        }

        post[j++]=(char)pop(p);
    }

    post[j]='\0';

    printf("Postfix Expression : %s\n",post);
}

int main(){

    ST s;
    char infix[100];
    char postfix[100];

    init(&s);

    printf("Enter infix expression: ");
    scanf("%s",infix);

    convert(&s,infix,postfix);

    free(s.a);

    return 0;
}
