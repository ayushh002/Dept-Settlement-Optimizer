#include "DebtSettlement.h"
#include <iostream>
#include <vector>

using namespace std;

void printTransactions(const vector<vector<string>>& transactions){
    for(const auto& t : transactions){
        cout<<t[0]<<" -> "<<t[1]<<" : "<<t[2]<<endl;
    }
    cout<<"Total transfers: "<<transactions.size()<<endl;
}

void runTestCase(const vector<vector<string>>& input, const string& description){
    cout<<"========== "<< description<<" =========="<<endl;
    cout<<"Input transactions:\n";
    for(auto& t : input){
        cout<<t[0]<<" owes "<<t[1]<<" "<<t[2]<<endl;
    }
    DebtSettlement solver;
    vector<vector<string>> result = solver.minimizeCashFlow(const_cast<vector<vector<string>>&>(input));
    cout<<"Optimized transfers:\n";
    printTransactions(result);
    cout<<endl;
}

int main() {
    // Test case 1: Provided example
    runTestCase({{"Tom", "Jerry", "1000"},
                 {"Jerry", "Spike", "1000"},
                 {"Spike", "Tom", "500"}}, "Example from problem");

    // Test case 2: Simple two-person debt
    runTestCase({{"Alice", "Bob", "250"}}, "Single transaction");

    // Test case 3: Circular debt with three persons
    runTestCase({{"A", "B", "100"},
                 {"B", "C", "100"},
                 {"C", "A", "100"}}, "Circular debt (net zero)");

    // Test case 4: Multiple debtors and creditors
    runTestCase({{"X", "Y", "200"},
                 {"X", "Z", "150"},
                 {"Y", "W", "100"},
                 {"Z", "W", "50"}}, "Multiple debtors/creditors");

    return 0;
}