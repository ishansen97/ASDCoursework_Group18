#pragma once
#include <string>
#include "CategoryExpenseSummary.h"
using namespace std;
class ExpenseBudget
{
private:
	string categoryName;
	CategoryExpenseSummary expenseSummary;

public:
	ExpenseBudget(string catName, CategoryExpenseSummary summary) : categoryName(catName), expenseSummary(summary) {}

	string getCategoryName() { return categoryName; }
	CategoryExpenseSummary getExpenseSummary() { return expenseSummary; }
};