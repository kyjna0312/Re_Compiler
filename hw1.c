#include<stdio.h>

int num;
enum {Null, NUMBER, PLUS, STAR, LPAREN, RPAREN, END} token;

void get_token(){

}

int expression(){
    int result;
    result = term();

    while(token == PLUS){
        get_token();
        result = result + term();
    }

    return(result);
}

int term(){
    int result;

    if(token == STAR){
        get_token();
        result = result*factor();
    }

    return(result);
}

int factor(){
    int result;

    if(token ==  NUMBER){
        result = num;
        get_token();
    }
    else if (token == LPAREN){
        get_token();
        result = expression();
        if(token == RPAREN){
            get_token();
        }
        else{
            error(2);
        }
    }
    else{
        error(1);
    }
    
    return(result);
}

void main(){
    int result;
    get_token();
    result = expression();

    if(token != END){
        error(3);
    }
    else{
        printf("%d\n", result);
    }
}

void error(int i){
    switch (i) {
        case 1 : printf("error: number or '(' expected \n"); break;
        case 2 : printf("error: ')' expected \n"); break;
        case 3 : printf("error: EOF expected \n"); break;
    }
    exit(1);
}