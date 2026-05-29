# Debt Settlement Optimizer

## Problem Overview
Given a list of transactions where person A owes person B amount X, we need to compute the minimum number of transfers required to settle all debts so that every person’s final balance becomes zero. The output is a list of simplified transactions that achieve this.

## Key Idea
- Compute net balance for each person: `balance = money received - money paid`.
- People with positive balance must receive money, those with negative balance must pay.
- The goal is to match payers and receivers with the fewest transfers.


## Mathematical Model
Let `B(p)` be the net balance of person `p`.  
We require `∑ B(p) = 0` (money conservation).  
A set of transfers `(u → v, amount)` modifies balances:  
`B(u) -= amount`, `B(v) += amount`.  
We want the minimum number of transfers such that all `B(p) = 0`.

## Algorithm Flow
1. **Compute net balances** from input transactions.
2. **Extract non-zero balances** and map them to indices.
3. **If number of non-zero balances ≤ 15**, use exact DFS/backtracking to find the minimum number of transfers (Optimal Account Balancing).
4. **Else** use a greedy two‑pointer matching (efficient, near‑optimal).
5. **Build output transactions** from the settlement plan.

## Example Walkthrough
Input:
Tom → Jerry 1000
Jerry → Spike 1000
Spike → Tom 500

Balances:
- Tom: +500 (received 1000, paid 500)
- Jerry: 0 (received 1000, paid 1000)
- Spike: -500 (paid 1000, received 500)

Only Tom (+500) and Spike (-500) remain. One transfer: Spike → Tom 500.  
Output: `[["Spike","Tom",500]]`


## Complexity Analysis
- **Balance computation:** O(T) where T = number of transactions.
- **Optimal (DFS):** O(N!) in worst case, but N ≤ 15 → feasible.
- **Greedy fallback:** O(M log M) where M = number of non‑zero balances.

**Space:** O(M) for balances.

## Edge Cases
- Single transaction → already settled.
- Circular debts (e.g., A→B, B→C, C→A) → net zero for all.
- Multiple debtors / creditors.
- Large amounts (up to 1e9 → use `long long`).
- Already settled persons (zero balance) → ignored.
- No non‑zero balances → empty output.

## Build Instructions
`g++ -std=c++17 src/*.cpp -o debt_optimizer
./debt_optimizer`

## Test Instructions
- Run the executable – it internally runs several sample test cases and prints the results.

## Assumptions
- Names are case‑sensitive unique strings.
- The total sum of balances is zero (input consistent).
- For optimal DFS, number of distinct non‑zero balance persons is assumed ≤15; otherwise, the greedy fallback is used.

## Future Improvements
- Use memoization (DP over subsets) for exact solution when N ≤ 20.
- Explore more advanced heuristics for large N.
- Parallel processing for DFS.

## Strategy Explanation for Reviewer
- Balance Calculation: We iterate through all input transactions, updating a map from person name to net balance (debtor -= amount, creditor += amount). This captures the net position of every individual.

- Settlement Generation: We extract all non‑zero balances.

- Exact DFS (Optimal Account Balancing) is used when the number of participants is small (≤15). It explores every possible pairing of a payer with a receiver, recursively settling the smallest absolute amount. This algorithm is known to produce the true minimum number of transactions, at the cost of exponential time.

- Greedy fallback (two‑pointer after sorting) is employed for larger sets because exact optimization is NP‑hard. The greedy approach often yields few transactions (bounded by #non‑zero − 1) and runs in O(M log M).

- Why this reduces unnecessary intermediate transactions: The DFS directly searches for the minimum number of transfers by merging opposite‑sign balances. The greedy algorithm eliminates at least one non‑zero balance per iteration, reducing total transfers.

- Efficiency trade‑off: Exact solution is only feasible for small M; for larger M (real‑world with many people), we accept a near‑optimal but much faster approximation. This balances correctness with scalability, following competitive programming best practices.

- Why this solution is efficient: Pre‑computation is linear. The fallback is linearithmic. The exact branch is only triggered for small M where factorial complexity is acceptable.

## Code execution
- cd src
- g++ -std=c++17 main.cpp DebtSettlement.cpp -o debt_optimizer.exe
- .\debt_optimizer.exe
