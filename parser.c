#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

enum type
{
    startexp,
    number,
    addition,
    subtract,
    multiply,
    divide,
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
typedef struct DoubleNode
{
    struct Token token;
    struct DoubleNode *left;
    struct DoubleNode *right;
} DoubleNode;

Token *tokenizeString(char *input);
DoubleNode *createTree(Token *tokenized);
int tokenLength(Token *tokenptr);
DoubleNode *doAddSub(Token *expression, DoubleNode *root);
DoubleNode *doMulDiv(Token *expression, DoubleNode *root);
DoubleNode *doPow(Token *expression, DoubleNode *root);
DoubleNode *doParen(Token *expression, DoubleNode *root);
DoubleNode *doNumber(Token *expression, DoubleNode *root);
DoubleNode *Node();
int compute(DoubleNode *root);
int computeO(DoubleNode *root,int o);
void printExpression(Token *ptr);
void printToken(Token token);

DoubleNode *Node()
{
    DoubleNode *node = malloc(sizeof(DoubleNode));
    node->token.type = 0;
    node->token.value = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void print2D(DoubleNode *root, int space)
{
    const char *types[] =
        {
            "startexp",
            "number",
            "addition",
            "subtract",
            "multiply",
            "divide",
            "power",
            "left_paren",
            "right_paren",
            "endexp"};
    if (root == NULL)
        return;
    space += 8;
    print2D(root->right, space);
    printf("\n");
    for (int i = 8; i < space; i++)
    {
        printf(" ");
    }
    printf("%s,%d\n", types[root->token.type], root->token.value);
    print2D(root->left, space);
}
void printToken(Token token)
{
    const char *types[] =
        {
            "startexp",
            "number",
            "addition",
            "subtract",
            "multiply",
            "divide",
            "power",
            "left_paren",
            "right_paren",
            "endexp"};

    if (token.type > 13)
    {
        printf("type: %d | value %d\n", token.type, token.value);
    }
    else
    {
        printf("type: %s | value %d\n", types[token.type], token.value);
    }
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

DoubleNode *createTree(Token *tokenized)
{
    DoubleNode *toreturn = Node();
    toreturn = doAddSub(tokenized, toreturn);
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
DoubleNode *doAddSub(Token *expression, DoubleNode *root)
{
    Token *ptr = expression;
    int parenCount = 0;
    while (ptr->type != endexp)
    {
        if (ptr->type == left_paren)
        {
            parenCount++;
        }
        if (ptr->type == right_paren)
        {
            parenCount--;
        }

        if ((ptr->type == subtract || ptr->type == addition) && parenCount == 0)
        {
            Token *leftExp = malloc((ptr - expression) * sizeof(Token) + sizeof(Token));
            printf("size %zu\n", ((ptr - expression) * sizeof(Token) + sizeof(Token)) / sizeof(Token));
            memcpy(leftExp, expression, (ptr - expression) * sizeof(Token));
            Token *endpointer = leftExp + (ptr - expression);
            endpointer->type = endexp;
            size_t size = (expression + tokenLength(expression) - ptr);
            Token *rightExp = malloc(size * sizeof(Token));
            memcpy(rightExp, ptr + 1, size * sizeof(Token) - sizeof(Token));
            endpointer = rightExp + size - 1;
            endpointer->type = endexp;

            if (root->token.type == 0)
            {
                root->token.type = ptr->type;
                root->left = Node();
                root->left = doMulDiv(leftExp, root->left);
                root = doAddSub(rightExp, root);
                return root;
            }
            else
            {
                DoubleNode *tempn = Node();
                tempn->token.type = ptr->type;
                tempn->left = root;
                root->right = Node();
                root->right = doMulDiv(leftExp, root->right);
                tempn = doAddSub(rightExp, tempn);
                return tempn;
            }

            return root;
        }
        ptr++;
    }
    if (root->token.type == 0)
    {
        return doMulDiv(expression, root);
    }
    else
    {
        root->right = Node();
        root->right = doMulDiv(expression, root->right);
        return root;
    }
}

DoubleNode *doMulDiv(Token *expression, DoubleNode *root)
{
    Token *ptr = expression;
    int parenCount = 0;
    while (ptr->type != endexp)
    {
        if (ptr->type == left_paren)
        {
            parenCount++;
        }
        if (ptr->type == right_paren)
        {
            parenCount--;
        }

        if ((ptr->type == divide || ptr->type == multiply) && parenCount == 0)
        {

            Token *leftExp = malloc((ptr - expression) * sizeof(Token) + sizeof(Token));
            printf("size %zu\n", ((ptr - expression) * sizeof(Token) + sizeof(Token)) / sizeof(Token));
            memcpy(leftExp, expression, (ptr - expression) * sizeof(Token));
            Token *endpointer = leftExp + (ptr - expression);
            endpointer->type = endexp;
            size_t size = (expression + tokenLength(expression) - ptr);
            Token *rightExp = malloc(size * sizeof(Token));
            memcpy(rightExp, ptr + 1, size * sizeof(Token) - sizeof(Token));
            endpointer = rightExp + size - 1;
            endpointer->type = endexp;
            if (root->token.type == 0)
            {
                root->token.type = ptr->type;
                root->left = Node();
                root->left = doPow(leftExp, root->left);
                root = doMulDiv(rightExp, root);
                return root;
            }
            else
            {
                DoubleNode *tempn = Node();
                tempn->token.type = ptr->type;
                tempn->left = root;
                root->right = Node();
                root->right = doPow(leftExp, root->right);
                tempn = doMulDiv(rightExp, tempn);
                return tempn;
            }

            return root;
        }
        ptr++;
    }
    if (root->token.type == 0)
    {
        return doPow(expression, root);
    }
    else
    {
        root->right = Node();
        root->right = doPow(expression, root->right);
        return root;
    }
}

DoubleNode *doPow(Token *expression, DoubleNode *root)
{
    Token *ptr = expression;
    int parenCount = 0;
    while (ptr->type != endexp)
    {
        if (ptr->type == left_paren)
        {
            parenCount++;
        }
        if (ptr->type == right_paren)
        {
            parenCount--;
        }

        if ((ptr->type == power) && parenCount == 0)
        {
            root->token.type = ptr->type;

            Token *leftExp = malloc((ptr - expression) * sizeof(Token) + sizeof(Token));
            printf("size %zu\n", ((ptr - expression) * sizeof(Token) + sizeof(Token)) / sizeof(Token));
            memcpy(leftExp, expression, (ptr - expression) * sizeof(Token));
            Token *endpointer = leftExp + (ptr - expression);
            endpointer->type = endexp;
            size_t size = (expression + tokenLength(expression) - ptr);
            Token *rightExp = malloc(size * sizeof(Token));
            memcpy(rightExp, ptr + 1, size * sizeof(Token) - sizeof(Token));
            endpointer = rightExp + size - 1;
            endpointer->type = endexp;
            root->left = Node();
            root->right = Node();
            root->left = doParen(leftExp, root->left);
            root->right = doPow(rightExp, root->right);
            return root;
        }
        ptr++;
    }
    return doParen(expression, root);
}

DoubleNode *doParen(Token *expression, DoubleNode *root)
{
    Token *ptr = expression;
    while (ptr->type != endexp)
    {
        if (ptr->type == left_paren)
        {
            Token *temptr = ptr + 1;
            int parenCount = 0;
            while (temptr->type != right_paren || parenCount != 0)
            {
                if (temptr->type == left_paren)
                {
                    parenCount++;
                }
                if (temptr->type == right_paren)
                {
                    parenCount--;
                }
                temptr++;
            }
            printf("yamaha\n");
            Token *leftExp = malloc((temptr - ptr) * sizeof(Token));
            memcpy(leftExp, ptr + 1, (temptr - ptr) * sizeof(Token) - sizeof(Token));
            Token *endpointer = leftExp + (temptr - ptr) - 1;
            printf("fitst %p last %p", leftExp, endpointer);
            endpointer->type = endexp;
            root->left = Node();
            if ((temptr + 1)->type == endexp)
            {
                root->right = NULL;
            }
            else
            {
                size_t size = (expression + tokenLength(expression) - temptr);

                Token *rightExp = malloc(size * sizeof(Token));
                memcpy(rightExp, temptr + 1, size * sizeof(Token) - sizeof(Token));
                endpointer = rightExp + size - 1;
                endpointer->type = endexp;
                root->right = Node();
                root->right = doParen(rightExp, root->right);
                return root;
            }
            ptr = temptr;

            root = doAddSub(leftExp, root->left);
        }
        ptr++;
    }
    return doNumber(expression, root);
}

DoubleNode *doNumber(Token *ptr, DoubleNode *root)
{

    if (ptr->type == number)
    {
        root->token.type = ptr->type;
        root->token.value = ptr->value;
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    else if (ptr->type == left_paren || ptr->type == right_paren)
    {
        printf("sad");
        return root;
    }
    printf("error");
    return root;
}

int computeO(DoubleNode *root,int o){
        if (root->token.type == number) {
            return root->token.value;
        }
        if (root == NULL) {
            return 0;
        }
        o = computeO(root->left,o);
        switch (root->token.type) {
            case addition:
                return o + computeO(root->right,o);
            case subtract:
                return o - computeO(root->right,o);
            case multiply:
                return o * computeO(root->right,o);
            case divide:
                return o / computeO(root->right,o);
        }
    }
int compute(DoubleNode *root){
    return computeO(root,0);
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
    DoubleNode *tree = createTree(expression);
    print2D(tree, 0);
    int answer = compute(tree);
    printf("answer is: %d\n",answer);
    free(expression);
    return 0;
}
