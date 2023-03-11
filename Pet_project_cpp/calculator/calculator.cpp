#include "calculator.h"


void calculator(char option, float a, float b)
{
    float result;

    switch(option)
    {
        case '+' :
            result = a + b;
            break;
        case '-' :
            result = a - b;
            break;
        case '*' :
            result = a * b;
            break;
        case '/' :
            result = a / b;
            break;
        default:
            cout << "Unexptected ERROR";
            return;
    }
    cout << "=======> Result: " << "a " << option << " b " << "= " << result << endl;
}