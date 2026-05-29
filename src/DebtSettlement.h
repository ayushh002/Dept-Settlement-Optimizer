#ifndef DEBT_SETTLEMENT_H
#define DEBT_SETTLEMENT_H

#include <vector>
#include <string>

class DebtSettlement{
public:
    std::vector<std::vector<std::string>> minimizeCashFlow(
        std::vector<std::vector<std::string>>& transactions);
};

#endif