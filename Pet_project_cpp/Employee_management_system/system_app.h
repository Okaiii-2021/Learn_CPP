#ifndef SYSTEM_APP_H
#define SYSTEM_APP_H

#include    "Header.h"
#include    "Employee_common_type.h"
#include    "Employeee.h"
#include    <vector>


class ManagementApp
{
private:
    /* data */
    std::vector <Employee> employees;

public:
    // Should use linked list or vector to keep track all data
    bool addRecord();
    bool viewRecord();
    bool updateRecord();
    bool deleteRecord();
};

#endif
