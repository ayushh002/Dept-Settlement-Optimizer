# Detailed Approach

## Problem Analysis

We are given a list of debts (A owes B amount X). After all transactions, each person has a net balance. We must find a new set of money transfers that brings every balance to zero **with the minimum number of transfers**. This is known as the *Minimum Cash Flow* or *Optimal Account Balancing* problem.

## Naïve Approach

One could simply reverse every debt, but that would not reduce the transaction count. Another naïve method is to repeatedly pick a debtor and a creditor and transfer money until all balances are zero. This can produce up to `(#debtors + #creditors - 1)` transactions but is far from optimal.

## Better Approach: Net Balance

- Compute for each person: `net = (received) - (paid)`.  
- Only persons with non‑zero net matter.  
- The problem reduces to: given a multiset of positive numbers (receivers) and negative numbers (payers) that sum to zero, find the minimum number of transfers. Each transfer can be seen as moving money from one person to another, reducing the absolute value of both balances.

## Chosen Algorithm

We implement **two strategies**:

### 1. Exact DFS (Optimal Account Balancing)
Used when the number of non‑zero balances `n ≤ 15`.  
- The DFS tries to settle the first unsettled person with every other person of opposite sign.  
- Each recursive call transfers the smaller absolute amount, reducing at least one balance to zero.  
- It explores all possibilities, keeping the minimum number of transactions.  
- The state space is O(n!) but practical for n ≤ 15.

### 2. Greedy Two‑Pointer Matching
Used for large `n` (the problem becomes NP‑hard).  
- Sort positive balances (receivers) and absolute negative balances (payers) in ascending order.  
- Repeatedly match the smallest positive with the smallest negative, transfer the minimum amount.  
- This runs in O(n log n) and typically yields a number of transfers close to the optimum (often `n-1` in the worst case but sometimes better).

**Why this combination?**  
The exact solution guarantees the minimum number of transactions but is exponential. For large real‑world datasets with many people, we accept a fast heuristic that still reduces transactions significantly.

## Correctness Proof

- **Balance zero after settlement:**  
  Each transfer subtracts from one balance and adds to another; total sum remains zero. The algorithm terminates only when all balances are zero.

- **Money conservation:**  
  Every transfer moves money from a negative balance to a positive balance, preserving the total sum (which is zero).

- **Minimum transactions (exact DFS):**  
  The DFS explores every possible way to pair debtors and creditors, counting the number of transfers. It returns the global minimum.

- **Greedy heuristic:**  
  No guarantee of optimality, but in practice it eliminates one non‑zero balance per iteration, so the number of transfers ≤ n-1. For many real distributions, it is very close to optimal.

## Complexity Analysis

- **Balance computation:** O(T) with T = number of input transactions.
- **Exact DFS:** O(n!) time, O(n) space (call stack). For n ≤ 15, at most ~1.3e12 states, but pruning reduces it drastically (often under a few million).
- **Greedy:** O(n log n) time, O(n) space.

## Dry Run (Exact DFS)

Example balances: [+5, -3, -2]  
Index 0 (+5) picks j=1 (-3): transfer min(5,3)=3 → balances become [+2, 0, -2]. Recurse from index 1: skip zero, now idx=2 (-2) with no opposite → backtrack. Then j=2 (-2): transfer 2 → [+3, -3, 0] → recurse idx=1 (-3) with j=2 zero? no opposite → backtrack. Minimum transfers found = 2.  
Indeed optimal: transfer 3 from 1→0 and 2 from 2→0 gives two transactions.

## Trade‑off Summary

- **Exact algorithm** is only feasible for small groups (≤15). It guarantees the absolute minimum number of transfers, which is valuable for small friend groups or sub‑problems.
- **Greedy fallback** scales to thousands of persons, completing in milliseconds. It may not always give the theoretical minimum, but for large groups the minimum is often impractical to compute, and the greedy solution is “good enough” in real‑world scenarios.

The code automatically switches between the two based on the number of non‑zero net balances.