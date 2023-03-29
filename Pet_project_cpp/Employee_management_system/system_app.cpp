#include    "system_app.h"


bool ManagementApp::addRecord()
{
    Employee    newEmployee;
    int gender;
    int position;

    cout << "Enter ID: ";
    cin  >> newEmployee.ID;
    cout << "Enter name: ";
    cin  >> newEmployee.name;
    cout << "Enter age: ";
    cin  >> newEmployee.age;

    cout << "Enter Gender <0. Female  1.Male> :";
    cin  >> gender;
    switch (gender)
    {
        case 0:
            newEmployee.gender = Gender::Female;
            break;
        case 1:
            newEmployee.gender = Gender::Male;
            break;
        default:
            cout << "Unkown gender";
            return false;
    }

    cout << "Enter Position <0. Manager  1. IT  2.HR  3. Guard> :";
    cin  >> position;

    switch (position)
    {
        case 0:
            newEmployee.position = Position::Manager;
            break;
        case 1:
            newEmployee.position = Position::IT;
            break;
        case 2:
            newEmployee.position = Position::HR;
            break;
        case 3:
            newEmployee.position = Position::Guard;
            break;
        default:
            cout << "Unkown position";
            return false;
    }
    cout << "Enter salary: ";
    cin  >> newEmployee.salary;

    employees.push_back(newEmployee);
    cout << "Add new employee info successfully !!!\n";
    return true;
};

bool ManagementApp::viewRecord()
{
    int ID;
    string gender[] = {"Male", "Female"};
    string position[] = {"Manager", "IT", "HR", "Guard"};

    cout << "===== Looking for Employee according to ID =====\n";
    cout << "Enter ID number: 1\n";
    cin  >> ID;

    for (int temp = 0; temp < employees.size(); temp++)
    {
        if (employees[temp].ID == ID)
        {
            cout << "ID: " << employees[temp].ID << endl;
            cout << "Name: " << employees[temp].name << endl;
            cout << "Age: " << employees[temp].age << endl;
            // cout << "Gender: " << static_cast<std::underlying_type_t<Gender>>(employees[temp].gender) << endl;
            cout << "Gender: " << gender[static_cast<int>(employees[temp].gender)] << endl;
            cout << "Position: " << position[static_cast<std::underlying_type_t<Position>>(employees[temp].position)] << endl;
            cout << "Salary: " << employees[temp].salary << endl;
            return true;
        }
    }

    cout << "Unknow ID\n";
    return false;
}

bool ManagementApp::updateRecord()
{
    int ID;
    int gender;
    int position;

    cout << "===== Update Employee infomation according to ID =====\n";
    cout << "Enter ID number";
    cin  >> ID;

    for (int temp = 0; temp < employees.size(); temp++)
    {
        if (employees[temp].ID == ID)
        {
            cout << "Enter Name: ";
            cin >> employees[temp].name;
            cout << "Enter Age: ";
            cin >> employees[temp].age;
            cout << "Enter Gender: \n 1. Male    0.Female\n";
            cin  >> gender;

            switch (gender)
            {
                case 0:
                    employees[temp].gender = Gender::Female;
                    break;
                case 1:
                    employees[temp].gender = Gender::Male;
                    break;
                default:
                    cout << "Unkown gender\n";
                    return false;
            }

            cout << "Enter Position: \n 1. Manager    2.IT   3.HR   4.Guard\n";
            cin  >> position;

            switch (position)
            {
                case 1:
                    employees[temp].position = Position::Manager;
                    break;
                case 2:
                    employees[temp].position = Position::IT;
                    break;
                case 3:
                    employees[temp].position = Position::HR;
                    break;
                case 4:
                    employees[temp].position = Position::Guard;
                    break;
                default:
                    cout << "Unkown position";
                    return false;
            }

            cout << "Enter Salary: ";
            cin >> employees[temp].salary;
            return true;
        }
    }
    cout << "Unkown ID";
    return false;
}

bool ManagementApp::deleteRecord()
{
    int ID;

    cout << "===== Delete Employee infomation according to ID =====\n";
    cout << "Enter ID number";
    cin  >> ID;

    // for (auto temp = employees.begin(); temp != employees.end(); temp++)
    // Should not use this way to loop over the vecotor 
    // because it is not safe because erasing an element invalidates the iterator
    // -> solution: you should use the return value of the erase() function,
    // which is the iterator to the next element after the one that was erased.
    // for (auto temp = employees.begin(); temp != employees.end(); ) 
    // but in this case we just need to find 1 ID so it is OKAY to use the old way

    for (auto temp = employees.begin(); temp != employees.end(); temp++)
    {
        if (temp->ID == ID)
        {
            employees.erase(temp);
            cout << "Removed !!!";
            return true;
        }
    }
     return false;
}








