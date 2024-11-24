#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPR_LENGTH 100

// Function prototypes
int evaluate_expression(const char *expression, int *result);
int precedence(char op);
int apply_operator(int a, int b, char op);
int is_digit(char ch);
int is_space(char ch);

int main() {
    char expression[MAX_EXPR_LENGTH];
    printf("Enter a mathematical expression: ");
    fgets(expression, MAX_EXPR_LENGTH, stdin);

    // Remove newline character
    expression[strcspn(expression, "\n")] = '\0';

    int result;
    if (evaluate_expression(expression, &result)) {
        printf("Result: %d\n", result);
    } else {
        printf("Error: Invalid expression or division by zero.\n");
    }
    return 0;
}

// Custom implementation of is_digit
int is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

// Custom implementation of is_space
int is_space(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
}

// Evaluate the mathematical expression
int evaluate_expression(const char *expression, int *result) {
    int numbers[MAX_EXPR_LENGTH];
    char operators[MAX_EXPR_LENGTH];
    int top_num = -1, top_op = -1;

    for (int i = 0; expression[i] != '\0'; ) {
        if (is_space(expression[i])) {
            i++; // Skip whitespace
            continue;
        }

        if (is_digit(expression[i])) {
            // Parse number
            int num = 0;
            while (is_digit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            numbers[++top_num] = num;
        } else if (strchr("+-*/", expression[i])) {
            // Resolve operators with higher precedence
            while (top_op >= 0 && precedence(operators[top_op]) >= precedence(expression[i])) {
                int b = numbers[top_num--];
                int a = numbers[top_num--];
                char op = operators[top_op--];

                if (op == '/' && b == 0) return 0; // Division by zero
                numbers[++top_num] = apply_operator(a, b, op);
            }
            // Push current operator
            operators[++top_op] = expression[i];
            i++;
        } else {
            return 0; // Invalid character
        }
    }

    // Apply remaining operators
    while (top_op >= 0) {
        int b = numbers[top_num--];
        int a = numbers[top_num--];
        char op = operators[top_op--];

        if (op == '/' && b == 0) return 0; // Division by zero
        numbers[++top_num] = apply_operator(a, b, op);
    }

    *result = numbers[top_num];
    return 1;
}

// Determine operator precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Apply the operator on two operands
int apply_operator(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}
