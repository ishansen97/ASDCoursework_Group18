#pragma once
#include "DataStore.h"
#include "ExpenseBudget.h"
#include "IncomeBudget.h"
using namespace std;
class BudgetService
{
private:
	DataStore* store;
	map<string, CategoryExpenseSummary*> getExpenseSummariesFromStore() {
		auto acctTypes = store->getAccountTypes()["Expense"];
		map<string, CategoryExpenseSummary*> categorySummaries = {};
		auto expenseTransactions = store->getTransactionsByAccountType(acctTypes);
		for (auto pair : store->getCategories(acctTypes))
		{
			string categoryName = pair.first;
			double expectedBudget = pair.second->getBudget();
			auto lambdaFunc = [categoryName](int initSum, const std::pair<int, Transaction*>& elem) {
				if (elem.second->getCategoryName() == categoryName)
					return initSum + elem.second->getAmount();
				return initSum;
			};

			double actualBudget = accumulate(expenseTransactions.begin(), expenseTransactions.end(), 0, lambdaFunc);
			CategoryExpenseSummary* summary = new CategoryExpenseSummary(pair.second, expectedBudget, actualBudget);
			categorySummaries.insert({ categoryName, summary });
		}

		return categorySummaries;
	}

	map<string, double> getIncomeSummariesFromStore() {
		auto acctTypes = store->getAccountTypes()["Income"];
		map<string, double> categorySummaries = {};

		auto incomeTransactions = store->getTransactionsByAccountType(acctTypes);
		for (auto pair : store->getCategories(acctTypes))
		{
			string categoryName = pair.first;
			auto lambdaFunc = [categoryName](int initSum, const std::pair<int, Transaction*>& elem) {
				if (elem.second->getCategoryName() == categoryName)
					return initSum + elem.second->getAmount();
				return initSum;
			};

			double actualBudget = accumulate(incomeTransactions.begin(), incomeTransactions.end(), 0, lambdaFunc);
			categorySummaries.insert({ categoryName, actualBudget });
		}

		return categorySummaries;
	}

	tuple<double, double, double> getBudgetTotalsFromStore() {
		double expectedSum = 0, actualSum = 0, incomeSum = 0;
		auto expenseSummaries = getExpenseSummariesFromStore();
		auto incomeSummaries = getIncomeSummariesFromStore();

		incomeSum = accumulate(incomeSummaries.begin(), incomeSummaries.end(), 0, [](int initialSum, const pair<string, double>& elem)
			{
				return initialSum + elem.second;
			});

		expectedSum = accumulate(expenseSummaries.begin(), expenseSummaries.end(), 0, [](int initialSum, const pair<string, CategoryExpenseSummary*>& elem)
			{
				return initialSum + elem.second->getExpectedBudget();
			});

		actualSum = accumulate(expenseSummaries.begin(), expenseSummaries.end(), 0, [](int initialSum, const pair<string, CategoryExpenseSummary*>& elem)
			{
				return initialSum + elem.second->getActualBudget();
			});

		return make_tuple(incomeSum, expectedSum, actualSum);
	}

	double calculateBalanceFromStore() {
		double totalIncome = 0, totalExpense = 0;
		auto incomeSummaries = getIncomeSummariesFromStore();
		auto expenseSummaries = getExpenseSummariesFromStore();
		totalIncome = accumulate(incomeSummaries.begin(), incomeSummaries.end(), 0, [](int initSum, const pair<string, double>& elem)
			{
				return initSum + elem.second;
			});

		totalExpense = accumulate(expenseSummaries.begin(), expenseSummaries.end(), 0, [](int initSum, const pair<string, CategoryExpenseSummary*>& elem)
			{
				return initSum + elem.second->getActualBudget();
			});

		return (totalIncome - totalExpense);
	}

public:
	BudgetService() { store = DataStore::createStore(); }

	vector<ExpenseBudget> getExpenseSummaries() {
		vector<ExpenseBudget> budgets;
		auto expenseSummaries = getExpenseSummariesFromStore();
		for (auto pair: expenseSummaries)
		{
			ExpenseBudget budget(pair.first, *pair.second);
			budgets.push_back(budget);
		}

		return budgets;
	}

	vector<IncomeBudget> getIncomeSummaries() {
		vector<IncomeBudget> budgets;
		auto incomeSummaries = getIncomeSummariesFromStore();
		for (auto pair: incomeSummaries)
		{
			IncomeBudget budget(pair.first, pair.second);
			budgets.push_back(budget);
		}

		return budgets;
	}

	tuple<double, double, double> getBudgetTotals() {
		return getBudgetTotalsFromStore();
	}

	double calculateBalance() {
		return calculateBalanceFromStore();
	}
};