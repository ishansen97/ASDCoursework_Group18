#pragma once
#include <string>
using namespace std;
class IncomeBudget
{
private:
	string categoryName;
	double incomeAmount;

public:
	IncomeBudget(string catName, double incomeAmt) : categoryName(catName), incomeAmount(incomeAmt) {}

	string getCategoryName() { return categoryName; }
	double getIncomeAmount() { return incomeAmount; }
};

