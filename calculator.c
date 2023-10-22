#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:6031)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//对于链栈的运用更加熟悉

typedef struct OPTRStackNode {
    char data;
    struct OPTRStackNode* next;
} OPTRStackNode, * OPTRLinkStack;

typedef struct OPEDStackNode {
    double data;
    struct OPEDStackNode* next;
} OPEDStackNode, * OPEDLinkStack;

void PushChar(OPTRLinkStack s, char x)
{
    OPTRStackNode* p = (OPTRStackNode*)malloc(sizeof(OPTRStackNode));
    p->data = x;
    p->next = s->next;
    s->next = p;
}

void PushDouble(OPEDLinkStack s, double x)
{
    OPEDStackNode* p = (OPEDStackNode*)malloc(sizeof(OPEDStackNode));
    p->data = x;
    p->next = s->next;
    s->next = p;
}

char PopChar(OPTRLinkStack s)
{
    if (s->next == NULL) {
        return '\0';
    }
    char x = s->next->data;
    OPTRStackNode* p = s->next;
    s->next = s->next->next;
    free(p);
    return x;
}

double PopDouble(OPEDLinkStack s)
{
    if (s->next == NULL) {
        return 0.0;
    }
    double x = s->next->data;
    OPEDStackNode* p = s->next;
    s->next = s->next->next;
    free(p);
    return x;
}

char GetOPTRTop(OPTRLinkStack s)
{
    if (s->next == NULL) {
        return '\0';
    }
    char ch = s->next->data;
    return ch;
}

double GetOPEDTop(OPEDLinkStack s)
{
    if (s->next == NULL) {
        return 0.0;
    }
    double num = s->next->data;
    return num;
}

int Index(char ch)
{
    char operators[8] = { ' ', '+', '-', '*', '/', '(', ')', '#' };
    for (int i = 0; i < 8; i++)
    {
        if (ch == operators[i])
        {
            return i;
        }
    }
    return 0;
}

char Precede(char topchar, char y)
{
    char result = '0';
    char Precedence_between_operators[8][8] = { { '0', '0','0','0','0','0','0','0' },
                                               { '0', '>','>','<','<','<','>','>' },
                                               { '0', '>','>','<','<','<','>','>' },
                                               { '0', '>','>','>','>','<','>','>' },
                                               { '0', '>','>','>','>','<','>','>' },
                                               { '0', '<','<','<','<','<','=','0' },
                                               { '0', '>','>','>','>','0','>','>' },
                                               { '0', '<','<','<','<','<','0','=' } };
    result = Precedence_between_operators[Index(topchar)][Index(y)];
    return result;
}

int isOperators(char ch)
{
    char operators[8] = { ' ', '+', '-', '*', '/', '(', ')', '#' };
    for (int i = 0; i < 8; i++)
    {
        if (ch == operators[i])
        {
            return 1;
        }
    }
    return 0;
}

double operate(double a, char operator, double b)
{
    double x = 0;
    switch (operator)
    {
    case '+':
    {
        x = a + b; break;
    }
    case '-':
    {
        x = a - b; break;
    }
    case '*':
    {
        x = a * b; break;
    }
    case '/':
    {
        x = a / b; break;
    }
    }
    return x;
}

double ExpressionCalculator(OPTRLinkStack OPTR, OPEDLinkStack OPED, char* inputChar)
{
    PushChar(OPTR, '#');
    int index = 0;
    char ch = inputChar[index++];
    while (ch != '#' || GetOPTRTop(OPTR) != '#')
    {
        if (!isOperators(ch))
        {
            double num = 0;
            double digit = 0.1;
            while (!isOperators(ch) && ch != '.')
            {
                if (ch >= '0' && ch <= '9')
                {
                    num = num * 10 + (ch - '0');
                }
                ch = inputChar[index++];
            }
            if (ch == '.')
            {
                ch = inputChar[index++];
                while (!isOperators(ch))
                {
                    if (ch >= '0' && ch <= '9')
                    {
                        num += digit * (ch - '0');
                        digit /= 10;
                    }
                    ch = inputChar[index++];
                }
            }
            PushDouble(OPED, num);
        }
        else
        {
            switch (Precede(GetOPTRTop(OPTR), ch))
            {
            case '<':
            {
                PushChar(OPTR, ch);
                ch = inputChar[index++];
                break;
            }
            case '>':
            {
                char op = PopChar(OPTR);
                double a = PopDouble(OPED);
                double b = PopDouble(OPED);
                PushDouble(OPED, operate(b, op, a));
                break;
            }
            case '=':
            {
                char ch1 = PopChar(OPTR);
                ch = inputChar[index++];
                break;
            }
            }
        }
    }
    return GetOPEDTop(OPED);
}

int main()
{
    OPTRLinkStack OPTR = (OPTRStackNode*)malloc(sizeof(OPTRStackNode));
    OPEDLinkStack OPED = (OPEDStackNode*)malloc(sizeof(OPEDStackNode));
    OPTR->next = NULL;
    OPED->next = NULL;
    char inputChar[100] = "";
    printf("请输入表达式：");
    scanf("%s", inputChar);
    printf("%lf\n", ExpressionCalculator(OPTR, OPED, inputChar));
    return 0;
}

