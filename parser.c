#include <stdio.h>
#include <math.h>
#include <stdlib.h>

enum type
{
    startexp,
    number,
    multiply,
    divide,
    addition,
    subtract,
    power,
    left_paren,
    right_paren,
    endexp
};

typedef struct Token
{
    enum type type;
    int value;
} Token;
typedef struct doubleNode
{
    struct Token token;
    DoubleNode *left;
    DoubleNode *right;
} DoubleNode;

void printToken(Token token)
{
    printf("type: %d value %d\n", token.type, token.value);
}
void printExpression(Token *ptr)
{
    while (ptr->type != endexp)
    {
        printToken(*ptr);
        ptr++;
    }
}

Token *tokenizeString(char *input)
{

    Token *ptr, *first, *end;
    int size = 3;
    ptr = (Token *)malloc(size * sizeof(Token));
    if (ptr == NULL)
    {
        printf("memorry error");
        exit(0);
    }
    end = ptr + size;
    first = ptr;

    while (*input)
    {
        if (ptr == end)
        {
            first = realloc(first, size * 2 * sizeof(Token));
            ptr = first + size;
            size *= 2;
            end = size + first;
        }
        if (*input == ' ')
        {
            input++;
            continue;
        }
        else if ((*input >= '0' && *input <= '9'))
        {
            ptr->type = number;
            ptr->value = *input - '0';
            input++;
            while ((*input >= '0' && *input <= '9'))
            {
                ptr->value = ((*input) - '0') + ptr->value * 10;
                input++;
            }
        }
        else if (*input == '/')
        {
            ptr->type = divide;
            input++;
        }
        else if (*input == '*')
        {
            ptr->type = multiply;
            input++;
        }
        else if (*input == '^')
        {
            ptr->type = power;
            input++;
        }
        else if (*input == '+')
        {
            ptr->type = addition;
            input++;
        }
        else if (*input == '-')
        {
            ptr->type = subtract;
            input++;
        }
        else if (*input == '(')
        {
            ptr->type = left_paren;
            input++;
        }
        else if (*input == ')')
        {
            ptr->type = right_paren;
            input++;
        }
        else
        {
            printf("unknown var\n");
            free(first);
            exit(0);
        }
        ptr++;
    }
    if (ptr == end)
    {
        first = realloc(first, (size + 1) * sizeof(Token));
        ptr = first + size;
    }
    ptr->type = endexp;
    return first;
}

DoubleNode createTree(Token *tokenized)
{
    DoubleNode toreturn = doAddSub(tokenized);
    return toreturn;
}
int tokenLength(Token *tokenptr)
{
    int counter = 0;
    while (tokenptr->type != endexp)
    {
        counter++;
        tokenptr++;
    }
    return counter;
}
DoubleNode doAddSub(Token *expression)
{
    Token *ptr = expression;
    DoubleNode root;
    while (ptr->type != endexp)
    {
        if (ptr->type == subtract || ptr->type == addition)
        {
            root.token.type = ptr->type;

            Token *leftExp = malloc(ptr - expression + sizeof(Token));
            memcpy(leftExp, expression, ptr - expression);
            Token *endpointer = leftExp + (ptr - expression);
            endpointer->type = endexp;
            size_t size = expression + tokenLength(expression) * sizeof(Token) - ptr;
            Token *rightExp = malloc(size);
            memcpy(rightExp, ptr + sizeof(Token),size-sizeof(Token));
            Token *endpointer = rightExp + size - sizeof(Token);
            endpointer->type = endexp;
            // Token leftExp[ptr - expression+1];

            // for (int i = 0;i<sizeof(leftExp)/sizeof(Token)-1;i++) {
            //     leftExp[i] = *(expression + i*sizeof(Token));
            // }

            // Token rightExp[expression+(tokenLength(expression)*sizeof(Token)+1) - ptr];
        }
        ptr++;
    }
}
int main()
{
    char input[100];
    printf("Hello, World!\n");
    scanf("%[^\n]%*c", input);
    printf("You entered: %s\n", input);
    Token *expression = tokenizeString(input);
    printExpression(expression);
    printf("yay\n");
    free(expression);
    return 0;
}