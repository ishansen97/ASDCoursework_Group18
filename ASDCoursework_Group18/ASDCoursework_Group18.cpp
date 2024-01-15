// ASDCoursework_Group18.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Program.h"
#include "BaseService.h"
using namespace std;

DataStore* DataStore::storeInstance = 0;
TransactionService Program::transactionService;
CategoryService Program::categoryService;
BudgetService Program::budgetService;
int main()
{
    Program::mainMenu();
    return 0;
}
