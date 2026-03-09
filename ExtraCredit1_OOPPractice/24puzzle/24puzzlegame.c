#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#define MAX_SOLUTIONS 3188
#define EASY_COUNT 7
double apply_operation(double a, double b, char operator) {
    switch (operator) {
        case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return (b) ? a / b : INT_MIN;
        default: return 0;
    }
}


void generateAllCombinations(int **numbers, char **operators) {
    char operators_list[] = "+-*/";
    int count = 0;
    for (short first_digit = 1; first_digit < 10; ++first_digit)
        for (short second_digit = 1; second_digit < 10; ++second_digit)
            for (short third_digit = 1; third_digit < 10; ++third_digit)
                for (short fourth_digit = 1; fourth_digit < 10; ++fourth_digit) {
                    bool found_for_this_set = false;
                    for (short op1 = 0; op1 < 4; ++op1) {
                        for (short op2 = 0; op2 < 4; ++op2) {
                            for (short op3 = 0; op3 < 4; ++op3) {
                                double result1 = apply_operation(first_digit, second_digit, operators_list[op1]);
                                double result2 = apply_operation(result1, third_digit, operators_list[op2]);
                                double result3 = apply_operation(result2, fourth_digit, operators_list[op3]);
                                if (result3 > 23.999999 && result3 < 24.00001 && count < MAX_SOLUTIONS) {
                                    numbers[count][0] = first_digit;
                                    numbers[count][1] = second_digit;
                                    numbers[count][2] = third_digit;
                                    numbers[count][3] = fourth_digit;
                                    operators[count][0] = op1;
                                    operators[count][1] = op2;
                                    operators[count][2] = op3;
                                    count ++;
                                    found_for_this_set = true;
                                }

                            }
                            if (found_for_this_set) break;
                        }
                        if (found_for_this_set) break;
                    }
                }
}

int main2(void) {




    return 0;
}