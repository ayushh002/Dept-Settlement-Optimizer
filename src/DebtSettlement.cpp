#include "DebtSettlement.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>

using namespace std;

class DebtSettlementImpl{
private:
    // Exact DFS (Optimal Account Balancing) for small N (≤15)
    int dfsMinTransfers(vector<long long>& balances, int idx, int cnt, int& best){
        int n=balances.size();
        // skip settled persons
        while(idx<n && balances[idx]==0) ++idx;
        if(idx==n){
            best = min(best, cnt);
            return cnt;
        }
        // Prune: already worse than best found
        if(cnt>=best) return INT_MAX;

        int minTrans=INT_MAX;
        // try to settle balances[idx] with any j>idx having opposite sign
        for (int j=idx+1; j<n; ++j){
            if (balances[idx]*balances[j]<0){
                // transfer from idx to j (or vice versa) the smaller absolute amount
                long long transfer=min(abs(balances[idx]), abs(balances[j]));
                balances[idx] -= transfer;
                balances[j] += transfer;

                int res=dfsMinTransfers(balances, idx+1, cnt+1, best);
                minTrans=min(minTrans, res);

                // backtrack
                balances[idx] += transfer;
                balances[j] -= transfer;
            }
        }
        return minTrans;
    }

    vector<vector<string>> buildTransactionsFromOptimal(
        vector<long long>& balances,
        vector<string>& persons,
        const vector<vector<int>>& moves){
        
        vector<vector<string>> result;
        vector<pair<pair<int,int>, long long>> recorded;
        recordTransfers(balances, 0, 0, recorded);
        for(auto& rec : recorded){
            result.push_back({persons[rec.first.first], persons[rec.first.second], to_string(rec.second)});
        }
        return result;
    }

    // Record optimal transfers during DFS (needs state capture)
    bool recordTransfers(vector<long long>& balances, int idx, int cnt, vector<pair<pair<int,int>, long long>>& moves) {
        int n=balances.size();
        while(idx<n && balances[idx]==0) ++idx;
        if(idx==n) return true;
        for(int j=idx+1; j<n; ++j){
            if(balances[idx]*balances[j]<0){
                long long transfer=min(abs(balances[idx]), abs(balances[j]));
                balances[idx] -= transfer;
                balances[j] += transfer;
                moves.push_back({{idx, j}, transfer});
                if(recordTransfers(balances, idx+1, cnt+1, moves)) return true;
                moves.pop_back();
                balances[idx] += transfer;
                balances[j] -= transfer;
            }
        }
        return false;
    }

    // Greedy two-pointer settlement (fallback for large N)
    vector<vector<string>> greedySettlement(vector<long long>& balances, vector<string>& persons){
        vector<pair<long long, int>> positives, negatives;
        for(int i=0; i<balances.size(); ++i){
            if(balances[i]>0) positives.emplace_back(balances[i], i);
            else if(balances[i]<0) negatives.emplace_back(-balances[i], i);
        }
        // sort ascending
        sort(positives.begin(), positives.end());
        sort(negatives.begin(), negatives.end());

        vector<vector<string>> result;
        int i=0, j=0;
        while(i<positives.size() && j<negatives.size()){
            long long amount=min(positives[i].first, negatives[j].first);
            // transfer from negative person (payer) to positive person (receiver)
            int payer=negatives[j].second;
            int receiver=positives[i].second;
            result.push_back({persons[payer], persons[receiver], to_string(amount)});

            positives[i].first -= amount;
            negatives[j].first -= amount;
            if (positives[i].first==0) ++i;
            if (negatives[j].first==0) ++j;
        }
        return result;
    }

public:
    vector<vector<string>> minimizeCashFlow(vector<vector<string>>& transactions){
        // Step 1: compute net balances
        unordered_map<string, long long> balanceMap;
        for(auto& t : transactions){
            string debtor = t[0];
            string creditor = t[1];
            long long amount = stoll(t[2]);
            balanceMap[debtor] -= amount;
            balanceMap[creditor] += amount;
        }

        // Step 2: extract non-zero balances
        vector<long long> balances;
        vector<string> persons;
        for(auto& entry : balanceMap){
            if(entry.second != 0){
                persons.push_back(entry.first);
                balances.push_back(entry.second);
            }
        }
        int n=balances.size();
        if (n==0) return {};

        // Step 3: choose algorithm
        if (n<=15){
            // Exact optimal using DFS
            int best = INT_MAX;
            dfsMinTransfers(balances, 0, 0, best);

            // Re-run to collect the actual transfers for the optimal solution
            vector<pair<pair<int,int>, long long>> moves;
            
            vector<long long> original = balances;
            recordTransfers(balances, 0, 0, moves);
            
            vector<vector<string>> result;
            for(auto& move : moves){
                result.push_back({persons[move.first.first], persons[move.first.second], to_string(move.second)});
            }
            
            vector<vector<string>> bestResult;
            int minTrans = INT_MAX;
            vector<long long> currBal = original;
            vector<pair<pair<int,int>, long long>> currMoves;
            function<void(int,int)> search = [&](int idx, int cnt){
                while(idx<n && currBal[idx]==0) ++idx;
                if (idx==n){
                    if(cnt < minTrans){
                        minTrans = cnt;
                        bestResult.clear();
                        for(auto& mv : currMoves){
                            bestResult.push_back({persons[mv.first.first], persons[mv.first.second], to_string(mv.second)});
                        }
                    }
                    return;
                }
                if(cnt>=minTrans) return;
                for(int j=idx + 1; j<n; ++j){
                    if(currBal[idx]*currBal[j]<0){
                        long long transfer = min(abs(currBal[idx]), abs(currBal[j]));
                        currBal[idx] -= transfer;
                        currBal[j] += transfer;
                        currMoves.push_back({{idx, j}, transfer});
                        search(idx+1, cnt+1);
                        currMoves.pop_back();
                        currBal[idx] += transfer;
                        currBal[j] -= transfer;
                    }
                }
            };
            search(0, 0);
            return bestResult;
        } 
        else{
            // Fallback: greedy
            return greedySettlement(balances, persons);
        }
    }
};

vector<vector<string>> DebtSettlement::minimizeCashFlow(vector<vector<string>>& transactions){
    DebtSettlementImpl impl;
    return impl.minimizeCashFlow(transactions);
}