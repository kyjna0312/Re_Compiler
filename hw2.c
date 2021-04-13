#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum
{
    false,
    true
} bool;

typedef struct _num
{
    bool type; // true: int,
    int int_val;
    double double_val;
} num;

void yyparse(); // int
void push(int i);
void shift(int i);
void reduce(int i);
void yyerror();
int yylex(); // int
void lex_error();

#define NUMBER 256
#define PLUS 257
#define STAR 258
#define LPAREN 259
#define RPAREN 260
#define END 261
#define EXPRESSION 0
#define TERM 1
#define FACTOR 2
#define ACC 999

int action[12][6] = {
    {5, 0, 0, 4, 0, 0},
    {0, 6, 0, 0, 0, ACC},
    {0, -2, 7, 0, -2, -2},
    {0, -4, -4, 0, -4, -4},
    {5, 0, 0, 4, 0, 0},
    {0, -6, -6, 0, -6, -6},
    {5, 0, 0, 4, 0, 0},
    {5, 0, 0, 4, 0, 0},
    {0, 6, 0, 0, 11, 0},
    {0, -1, 7, 0, -1, -1},
    {0, -3, -3, 0, -3, -3},
    {0, -5, -5, 0, -5, -5}};

int go_to[12][3] = {
    {1, 2, 3},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {8, 2, 3},
    {0, 0, 0},
    {0, 9, 3},
    {0, 0, 10},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

int prod_left[7] = {0, EXPRESSION, EXPRESSION, TERM, TERM, FACTOR, FACTOR};
int prod_length[7] = {0, 3, 1, 3, 1, 3, 1};

int stack[1000];
num value[1000];
int top = -1;
int sym;
char yytext[32];
num yylval;
bool warning_flag;
int cnt;

int main(void)
{
    yyparse();
    return 0;
}

void yyparse()
{
    int i;
    stack[++top] = 0;

    printf("> ");
    sym = yylex();
    do
    {
        i = action[stack[top]][sym - 256];
        if (i == ACC)
        {
            if (warning_flag)
                printf("waring: integer and float operate together! at: %d\n", cnt);

            if (value[top].type) // 정수인 경우
                printf("> %d\n", value[top].int_val);
            else
                printf("> %lf\n", value[top].double_val);
        }

        else if (i > 0)
            shift(i);
        else if (i < 0)
            reduce(-i);
        else
            yyerror("");
    } while (i != ACC);
}

void push(int i)
{
    top++;
    stack[top] = i;
}

void shift(int i)
{
    push(i);
    value[top] = yylval;
    sym = yylex();
}

void reduce(int i)
{
    int old_top;
    top -= prod_length[i];
    old_top = top;
    push(go_to[stack[old_top]][prod_left[i]]);

    switch (i)
    {
    case 1:
        if (value[old_top + 1].type ^ value[old_top + 3].type) // 실수 정수가 섞여있다면
        {
            warning_flag = true;
            num val;
            val.type = false;
            val.double_val = value[old_top + 1].type ? (double)value[old_top + 1].int_val + value[old_top + 3].double_val : value[old_top + 1].double_val + (double)value[old_top + 3].int_val;
            value[top] = val;
        }

        else // 둘 다 실수이거나 정수이거나
        {
            num val;
            val.type = value[old_top + 1].type;
            if (val.type) // 정수인 경우
                val.int_val = value[old_top + 1].int_val + value[old_top + 3].int_val;

            else
                val.double_val = value[old_top + 1].double_val + value[old_top + 3].double_val;

            value[top] = val;
        }
        break;
    case 2:
        value[top] = value[old_top + 1];
        break;
    case 3:
        // 실수 정수 인지 판단 후 곱셈
        if (value[old_top + 1].type ^ value[old_top + 3].type) // 실수 정수가 섞여있다면
        {
            warning_flag = true;
            num val;
            val.type = false;
            val.double_val = value[old_top + 1].type ? (double)value[old_top + 1].int_val * value[old_top + 3].double_val : value[old_top + 1].double_val * (double)value[old_top + 3].int_val;
            value[top] = val;
        }

        else // 둘 다 실수이거나 정수이거나
        {
            num val;
            val.type = value[old_top + 1].type;
            if (val.type) // 정수인 경우
                val.int_val = value[old_top + 1].int_val * value[old_top + 3].int_val;

            else
                val.double_val = value[old_top + 1].double_val * value[old_top + 3].double_val;

            value[top] = val;
        }
        break;
    case 4:
        value[top] = value[old_top + 1];
        break;
    case 5:
        value[top] = value[old_top + 2];
        break;
    case 6:
        value[top] = value[old_top + 1];
        break;
    default:
        yyerror("parsing table error");
        break;
    }
}

// 과제1처럼 error를 숫자로 넘겨주기?
void yyerror(char *str)
{
    if (!strcmp(str, "parsing table error"))
    {
        printf("parsing table error\n");
    }

    else
    {
        printf("syntax error\n");
    }
    exit(1);
}

int yylex()
{
    static char ch = ' ';
    int i = 0;

    while (ch == ' ' || ch == '\t')
    {
        ch = getchar();
    }

    if (isdigit(ch))
    {
        do
        {
            yytext[i++] = ch;
            ch = getchar();
            if (!warning_flag)
                cnt++;
        } while (isdigit(ch));

        if (ch == '.') // 실수 일 때
        {
            do
            {
                yytext[i++] = ch;
                ch = getchar();
                if (!warning_flag)
                    cnt++;
            } while (isdigit(ch));

            yytext[i] = 0;
            yylval.type = false; // 실수
            yylval.double_val = atof(yytext);
            cnt++;
            return NUMBER;
        }

        yytext[i] = 0;
        yylval.type = true; // 정수
        yylval.int_val = atoi(yytext);
        return NUMBER;
    }
        else if (ch == '+')
    {
        ch = getchar();
        return PLUS;
    }

    else if (ch == '*')
    {
        ch = getchar();
        return STAR;
    }

    else if (ch == '(')
    {
        ch = getchar();
        return LPAREN;
    }
    else if (ch == ')')
    {
        ch = getchar();
        return RPAREN;
    }
    else if (ch == '\n')
    {
        return END;
    }

    else
        lex_error();

    return -1;
}

void lex_error()
{
    printf("illegal token\n");
    exit(1);
}

