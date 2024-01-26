#pragma once
#include "DataStore.h"
#include "Transaction.h"
#include "BaseService.h"
#include <iostream>
#include "BudgetService.h"
#include "IncomeBudget.h"
#include "ExpenseBudget.h"
#include <sstream>
using namespace std;

class Program
{
private:
	static TransactionService transactionService;
	static CategoryService categoryService;
	static BudgetService budgetService;
#pragma region Handling user options

	static void handleUserOption(int option) {
		switch (option)
		{
		case 1:
			displayTransactionMenu();
			break;
		case 2:
			displayCategoryMenu();
			break;
		case 3:
			trackProgress();
			break;
		default:
			break;
		}
	}

	static void handleTransactionOption(int option) {
		switch (option)
		{
		case 1:
			addTransaction();
			break;
		case 2:
			viewRecentTransactions();
			break;
		case 3:
			editTransaction();
			break;
		case 4:
			deleteTransaction();
			break;
		default:
			break;
		}
	}

	static void handleCategoryOption(int option) {
		switch (option)
		{
		case 1:
			addCategory();
			break;
		case 2:
			viewCategories();
			break;
		case 3:
			setCategoryBudget();
			break;
		default:
			break;
		}
	}
#pragma endregion

#pragma region Displaying menu items
	static void displayTransactionMenu() {
		int transactionOption;
		do
		{
			cout << "Manage your transactions here." << endl;
			cout << "1. Add new transaction" << endl;
			cout << "2. View recent transactions." << endl;
			cout << "3. Edit transaction" << endl;
			cout << "4. Delete transaction" << endl;
			cout << "5. Main Menu" << endl;
			cout << "Enter your option: ";
			cin >> transactionOption;

			if (isInvalidOption(transactionOption, 1, 5))
				clearConsoleAndPrintMessage("Invalid input. Please try again!", &transactionOption);
			else
				handleTransactionOption(transactionOption);
		} while (transactionOption != 5);
	}

	static void displayCategoryMenu() {
		int categoryOption;
		do
		{
			cout << "Manage the categories here." << endl;
			cout << "1. Add new category" << endl;
			cout << "2. View category list." << endl;
			cout << "3. Specify budgets for categories." << endl;
			cout << "4. Main Menu" << endl;
			cout << "Enter your option: ";
			cin >> categoryOption;

			if (isInvalidOption(categoryOption, 1, 4))
				clearConsoleAndPrintMessage("Invalid input. Please try again!", &categoryOption);
			else
				handleCategoryOption(categoryOption);
		} while (categoryOption != 4);
	}
#pragma endregion

#pragma region Transaction Operations
	static void addTransaction() {
		int transactionType = -1;
		int recurring = -1;
		bool type;
		double amount = -1;
		bool isRecurring;
		string categoryName, note;
		Category* category;

		cout << "===== Enter transaction details: =====" << endl;

		while (!isValidFieldOption(transactionType, { 0,1 }))
		{
			cout << "Transaction type (Expense/Income) (Enter 0 for Expense, 1 for Income): " << endl;
			cin >> transactionType;

			if (!isValidFieldOption(transactionType, { 0,1 })) {
				clearConsoleAndPrintMessage("Invalid transaction type. Please try again.", &transactionType, -1);
			}
			type = transactionType == 0;
		}

		while (isInvalidNumber(amount))
		{
			cout << "Amount: " << endl;
			cin >> amount;

			if (isInvalidNumber(amount)) {
				clearConsoleAndPrintMessage("Invalid amount. Please try again later.", &amount, -1);
			}
		}

		while (!isValidFieldOption(recurring, { 0,1 }))
		{
			cout << "Recurring?: (Enter 1 for Recurring, else 0)" << endl;
			cin >> recurring;
			if (!isValidFieldOption(recurring, { 0,1 }))
				clearConsoleAndPrintMessage("Invalid input for Recurring field.", &recurring, -1);
			isRecurring = recurring == 1;
		}

		cout << "Note: " << endl;
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		getline(cin, note);

		while (!isExistingCategoryName(categoryName))
		{
			cout << "Category: " << endl;
			getline(cin, categoryName);
			if (!isExistingCategoryName(categoryName))
				cout << "Invalid category name. Please try again." << endl;
		}

		category = categoryService.getCategory(categoryName);
		int transactionId = transactionService.getLastTransactionId() + 1;

		Transaction* transaction = new Transaction(transactionId, amount, type, note, category, isRecurring);

		transactionService.addTransaction(transactionId, transaction);
		cout << "Transaction added successfully." << endl;
	};

	static void viewRecentTransactions() {
		cout << "===== Transaction list =====" << endl;
		for (auto pair : transactionService.getItems())
		{
			pair->printDetails();
			cout << "=========" << endl;
		}
	}

	static void editTransaction() {
		int id = -1, newAmount = -1, recurring;
		string strAmount, newNote, recurringInput;

		while (isInvalidNumber(id) || !isValidTransaction(id))
		{
			cout << "Please enter transaction ID to edit: " << endl;
			cin >> id;
			if (isInvalidNumber(id)) {
				clearConsoleAndPrintMessage("Invalid input. Please try again.", &id, -1);
			}
			if (!isValidTransaction(id)) {
				clearConsoleAndPrintMessage("Invalid transaction Id. Please try again.", &id, -1);
			}
		}

		Transaction* transaction = transactionService.getTransaction(id);

		cout << "Enter new amount (Press ENTER to skip):";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, strAmount);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Enter new note (Press ENTER to skip):";
		getline(cin, newNote);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Recurring? (Enter 1 for Recurring, else 0)  (Press ENTER to skip):";
		getline(cin, recurringInput);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// setting new values
		if (!strAmount.empty() && tryParseInt(strAmount, newAmount)) {
			transaction->setAmount(newAmount);
		}
		if (!newNote.empty()) {
			transaction->setNote(newNote);
		}
		if (!recurringInput.empty() && tryParseInt(recurringInput, recurring))
			transaction->setRecurring(recurring);

		transaction->printDetails();
	}

	static void deleteTransaction() {
		int id = -1;

		while (isInvalidNumber(id) || !isValidTransaction(id))
		{
			cout << "Please enter transaction ID to edit: " << endl;
			cin >> id;
			if (isInvalidNumber(id)) {
				clearConsoleAndPrintMessage("Invalid number. Please try again.", &id, -1);
			}
			if (!isValidTransaction(id)) {
				clearConsoleAndPrintMessage("Invalid transaction Id. Please try again.", &id, -1);
			}
		}

		transactionService.deleteTransaction(id);
		cout << "Transaction deleted successfully." << endl;
	}
#pragma endregion

#pragma region Category Operations
	static void addCategory() {
		string categoryName;
		double budgetAmt = -1;
		int acctType = -1;
		bool isExistingCategory = false;
		int categoryInputPromptCount = 0;

		while (isExistingCategoryName(categoryName) || categoryInputPromptCount == 0)
		{
			cout << "Enter category name: " << endl;
			cin >> categoryName;
			if (isExistingCategoryName(categoryName)) {
				clearConsoleAndPrintMessage("This category name already exists. Please try again.");
			}
			categoryInputPromptCount++;
		}

		while (isInvalidNumber(budgetAmt))
		{
			cout << "Initial budget amount: " << endl;
			cin >> budgetAmt;
			if (isInvalidNumber(budgetAmt)) {
				clearConsoleAndPrintMessage("Invalid input. Please try again.", &budgetAmt, -1);
			}
		}

		while (!isValidFieldOption(acctType, { 0,1 }))
		{
			cout << "Account Type (Expense = 0, Income = 1): " << endl;
			cin >> acctType;
			if (!isValidFieldOption(acctType, { 0,1 })) {
				clearConsoleAndPrintMessage("Invalid input. Please try again.", &acctType, -1);
			}
		}

		Category* newCategory = new Category(categoryName, budgetAmt, acctType);
		categoryService.addCategory(categoryName, newCategory);

		cout << "Category added successfully." << endl;
	}

	static void viewCategories() {
		cout << "===== List of categories =====" << endl;
		for (auto category : categoryService.getItems())
		{
			category->printDetails();
			cout << "-------------------------" << endl;
		}

		cout << "==============================" << endl;
	}

	static void setCategoryBudget() {
		for (auto expenseCategory : categoryService.getItems(categoryService.getAccountTypes()["Expense"]))
		{
			double budget = -1;
			cout << "===============" << endl;
			cout << "Category: " << expenseCategory->getName() << endl;
			cout << "Current budget: " << expenseCategory->getBudget() << endl;
			cout << "New budget: ";
			while (isInvalidNumber(budget))
			{
				cin >> budget;
				if (isInvalidNumber(budget))
					clearConsoleAndPrintMessage("Invalid input. Please try again.", &budget, -1);
				cout << "===============" << endl;
			}

			expenseCategory->setBudget(budget);
		}
		cout << "Budgets have been set." << endl;
	}
#pragma endregion

#pragma region Track Progress operations

	static void trackProgress() {
		string incomeHeader = "INCOME";
		string incomeAmountHeader = "AMOUNT";
		string header1 = "EXPENSE";
		string header2 = "EXPECTED BUDGET";
		string header3 = "ACTUAL BUDGET";
		string expenseTotalHeader = "EXPENSE TOTAL";
		string incomeTotalHeader = "INCOME TOTAL";
		string balanceHeader = "BALANCE";
		// display income
		cout << string(80, '=') << endl;
		cout << incomeHeader << string(20 - incomeHeader.length(), ' ') << incomeAmountHeader << endl;
		for (auto incomeSummary : budgetService.getIncomeSummaries())
		{
			string categoryName = incomeSummary.getCategoryName();
			double amount = incomeSummary.getIncomeAmount();
			cout << categoryName << string(20 - categoryName.length(), ' ') << amount << endl;
		}

		// display expenses
		cout << string(80, '=') << endl;
		cout << header1 << string(20 - header1.length(), ' ') << header2 << string(30 - header2.length(), ' ') << header3 << endl;
		for (auto &expenseSummary : budgetService.getExpenseSummaries())
		{
			string categoryName = expenseSummary.getCategoryName();
			CategoryExpenseSummary summary = expenseSummary.getExpenseSummary();
			cout << categoryName << string(20 - categoryName.length(), ' ') << summary.getExpectedBudget() << string(40 - to_string(summary.getExpectedBudget()).length(), ' ') << summary.getActualBudget() << endl;
		}

		cout << string(80, '=') << endl;
		// get the budget totals
		auto budgetTotals = budgetService.getBudgetTotals();
		cout << incomeTotalHeader << string(20 - incomeTotalHeader.length(), ' ') << get<0>(budgetTotals) << endl;
		cout << expenseTotalHeader << string(20 - expenseTotalHeader.length(), ' ') << get<1>(budgetTotals) << string(40 - to_string(get<0>(budgetTotals)).length(), ' ') << get<2>(budgetTotals) << endl;

		// get the balance
		double balance = budgetService.calculateBalance();
		cout << balanceHeader << string(20 - balanceHeader.length(), ' ') << balance << endl;
		cout << string(80, '=') << endl;
	}
#pragma endregion

#pragma region helpers
	static bool isInvalidOption(int option, int low, int high) {
		return (cin.fail() || option < low || option > high);
	}

	static void clearConsoleAndPrintMessage(string message, int* optionPtr, int value = 0) {
		cin.clear();  // Clear the error flag
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << message << endl;
		*optionPtr = value;
	}

	static void clearConsoleAndPrintMessage(string message, double* optionPtr, int value = 0) {
		cin.clear();  // Clear the error flag
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << message << endl;
		*optionPtr = value;
	}

	static void clearConsoleAndPrintMessage(string message) {
		cin.clear();  // Clear the error flag
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << message << endl;
	}

	static bool isExistingCategoryName(string catName) {
		return categoryService.isCategoryAvailable(catName);
	}

	static bool isValidTransaction(int transactionId) {
		return transactionService.isTransactionAvailable(transactionId);
	}

	static bool isValidFieldOption(int input, const int(&options)[]) {
		if (isInvalidNumber(input))
			return false;

		int size = sizeof(*options);
		return (find(options, options + size, input) != options + size);
	}

	static bool isInvalidNumber(double input) {
		return (cin.fail() || input < 0);
	}

	static bool tryParseInt(const string& str, int& output) {
		std::istringstream iss(str);
		iss >> output;

		return iss.eof() && !iss.fail();
	}
#pragma endregion


public:
	static void mainMenu() {
		int userOption;
		do
		{
			cout << "Welcome to Expense tracking and Budgeting Application." << endl;
			cout << "Please select your option." << endl;
			cout << "\t 1. Manage Transactions." << endl;
			cout << "\t 2. Manage Categories." << endl;
			cout << "\t 3. Track your progress." << endl;
			cout << "\t 4. Exit." << endl;
			cout << "Enter your option: ";
			cin >> userOption;

			if (isInvalidOption(userOption, 1, 4)) {
				clearConsoleAndPrintMessage("Invalid input. Please try again!", &userOption);
			}
			else
				handleUserOption(userOption);

		} while (userOption != 4);
	}
};