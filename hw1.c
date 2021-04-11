#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

void get_token();
double expreesion();
double term();
double factor();
void error(int i);

char buf[100];
char BUF;
int i = 0;
double num;
enum {Null, NUMBER, PLUS, STAR, LPAREN, RPAREN, END} token;

void get_token(){
    if(BUF != '\0'){
        switch(BUF){
            case '+' : token = PLUS; break;
            case '*' : token = STAR; break;
            case '(' : token = LPAREN; break;
            case ')' : token = RPAREN; break;
            case '\n' : token = EOF; break;
        }
        BUF = '\0';
    }
    else{
        char ch = getchar();

        if(isdigit(ch)){
            //1. 문자열 숫자 저장
            do{
                buf[i] = ch;
                i++;
                ch = getchar();
            }while(isdigit(ch));

            //2. 숫자 변환
            if(strchr(buf, '.')){
                num = atof(buf);
                printf("%f\n", num);
            }
            else{
                num = atoi(buf);
                printf("%f\n", num);
            }

            //3. 버퍼 초기화
            BUF = ch;
            token = NUMBER;
            buf[0] = '\0';
            i = 0;
        }
        else{
            switch(ch){
                case '+' : token = PLUS; break;
                case '*' : token = STAR; break;
                case '(' : token = LPAREN; break;
                case ')' : token = RPAREN; break;
                case '\n' : token = EOF; break;
            }   
        }
    }
}

double expression(){
    double result;
    result = term();

    while(token == PLUS){
        get_token();
        result = result + term();
    }

    return(result);
}

double term(){
    double result;
    result = factor();

    if(token == STAR){
        get_token();
        result = result * factor();
    }

    return(result);
}

double factor(){
    double result;

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
    double result;
    get_token();
    result = expression();

    if(token != END){
        error(3);
    }
    else{
        printf("%lf\n", result);
    }
}

void error(int i){
    switch (i) {
        case 1 : printf("error: number or '(' expected \n"); break;
        case 2 : printf("error: ')' expected \n"); break;
        case 3 : printf("error: EOF expected \n"); break;
        case 4 : printf("error: Not definition charactor\n"); break;
    }
    exit(1);
}
