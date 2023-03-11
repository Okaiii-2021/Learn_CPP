#include "Header.h"
#include "calculator.h"


int main()
{
    float a, b;
    char option;

    while (option != STOP){
        cout << "========== Calculator ==========\n";
        cout << " +\n -\n *\n /\n 1 -> Stop\n";
        cout << "Enter your option: ";
        cin >> option;

        switch(option)
            {
                case '+':
                case '-':
                case '*':
                case '/':
                    cout << "Enter a: ";
                    cin >> a;
                    cout << "Enter b: ";
                    cin >> b;
                    calculator(option, (float) a, (float) b);
                    break;
                case '1' :
                    cout << "****** End program!!! ******";
                    break;
                default:
                    cout << "Invalid option!!!" << endl;
            }
    }
    return 0;
}