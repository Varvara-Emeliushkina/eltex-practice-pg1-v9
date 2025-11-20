#ifndef FUNCTION_INTERFACE_H
#define FUNCTION_INTERFACE_H

typedef double (*math_function_t)(double, double);

typedef struct {
    char name[50];
    char symbol[10];
    math_function_t function;
} FunctionInfo;

#endif