#pragma once
#include <map>
#include "DataStore.h"
using namespace std;

template <typename T>
class BaseService
{
protected:
	DataStore* store;

public:
	BaseService() { store = DataStore::createStore(); }

	map<string, int> getAccountTypes() { return store->getAccountTypes(); }
	vector<T> getItems() {}
	vector<T> getItems(int accountType) {}
};

class TransactionService : public BaseService<Transaction> {
public:
	TransactionService() : BaseService() {}

	vector<Transaction*> getItems() { 
		auto storeTransactions = store->getTransactions();
		vector<Transaction*> transactions;
		int index = 0;
		for (auto pair : storeTransactions) {
			transactions.push_back(pair.second);
		}
		return transactions; 
	}

	vector<Transaction*> getItems(int accountType) { 
		auto storeTransactions = store->getTransactionsByAccountType(accountType);
		vector<Transaction*> transactions;
		int index = 0;
		for (auto pair : storeTransactions) {
			transactions.push_back(pair.second);
		}
		return transactions;
	}

	void addTransaction(int id, Transaction* transaction) { store->addTransaction(id, transaction); }
	Transaction* getTransaction(int id) { return store->getTransaction(id); }
	int getLastTransactionId() { return store->getLastTransactionId(); }
	void deleteTransaction(int transactionId) { store->deleteTransaction(transactionId); }
	bool isTransactionAvailable(int transactionId) { return store->isTransactionAvailable(transactionId); }
};

class CategoryService : public BaseService<Category> {
public:
	CategoryService() : BaseService() {}
	vector<Category*> getItems() {
		auto storeCategories = store->getCategories();
		vector<Category*> categories;
		int index = 0;
		for (auto pair : storeCategories) {
			categories.push_back(pair.second);
		}
		return categories;
	}

	vector<Category*> getItems(int accountType) {
		auto storeCategories = store->getCategories(accountType);
		vector<Category*> categories;
		int index = 0;
		for (auto pair : storeCategories) {
			categories.push_back(pair.second);
		}
		return categories;
	}

	void addCategory(string catName, Category* category) { store->addCategory(catName, category); }
	Category* getCategory(string catName) { return store->getCategory(catName); }
	bool isCategoryAvailable(string catName) { return store->isCategoryAvailable(catName); }
};