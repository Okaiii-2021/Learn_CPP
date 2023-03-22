#include    "Header.h"
#include    "system_app.h"


int main()
{
    // Use interface
    ManagementApp  App;
    int option;

    while(true)
    {
        cout << "----- M A N A G E M E N T    A P P L I C A T I O N -----\n";
        cout << "               --- M A I N  M E N U ---                 \n";
        cout << "\n\n";
        cout << "           1. ADD NEW EMPLOYEE\n";
        cout << "           2. UPDATE EMPLOYEE INFOMATION\n";
        cout << "           3. DISPLAY EMPLOYY INFOMATION\n";
        cout << "           4. DELETE EMPLOYY INFOMATION\n";
        cout << "           5. EXIT\n";
        cout << "\n";

        cout << "Enter your option: ";
        cin >> option;

        switch(option)
        {
            case 1:
                App.addRecord();
                break;
            case 2:
                App.updateRecord();
                break;
            case 3:
                App.viewRecord();
                break;
            case 4:
                App.deleteRecord();
                break;
            case 5:
                return 0;
            default:
                cout << "Invalid choice !!!!\n";
                cout << "Please enter your choice again !!!";
        }
    }
    return 0;
}