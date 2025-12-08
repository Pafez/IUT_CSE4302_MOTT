# Money On The Table (MOTT)

A debt management system for roommates, friends, and family who frequently share expenses.

## Overview

MOTT automatically tracks shared expenses and debts between groups, eliminating miscalculations, disagreements, and forgotten payments. The system optimizes transactions to minimize the number of payments needed to settle all debts.

## Problem

Managing shared expenses with roommates or friends is tedious:
- Manual tracking leads to miscalculations and miscommunication
- People forget debts or payments
- Circular debts create unnecessary transactions
- No proof of transactions causes disputes

## Solution

MOTT provides a centralized "Table" system where:
- Each person has an account linked to the Table
- Users create debt entries that require mutual confirmation
- The system automatically reduces transactions by optimizing debt settlement
- Complete transaction history provides proof and accountability

## Features

### Core
- **Table System**: Central hub for group expense tracking
- **Account Management**: Individual accounts with positive/negative balances
- **Entry Creation**: Add debts owed or received
- **Mutual Confirmation**: Entries require acceptance from all parties
- **Transaction Log**: Complete history of all entries
- **Debt Optimization**: Automatic calculation to minimize total transactions

### Planned (Stretch Goals)
- Entry templates for common expense types
- Multiple Tables per user (different friend groups)
- Monthly expense alerts
- Recurring entries (e.g., rent, subscriptions)
- Client-server architecture

## Use Case

### The CDS Rush

Five. Minutes. That's all it takes. After the East shop shutdown, all the traffic from both shops in the CDS had to be handled by the West shop. One's lucky if class finishes earlier than 1:00 PM—normal service, no hassle. But get there at even 1:05 and consider the whole lunch hour wasted standing in lines. 

A brave soldier is sent with the rest of his friends' orders. He has to pay for it all at that moment, but he knows his friends will pay him back later—he trusts them.

But how does he keep track? **The MOTT system!** 

1. Opens the program and creates entries against his friends' accounts
2. Friends confirm the amounts
3. System automatically offsets against his old debts
4. At any time, he can request a report showing exactly who owes what

He won't have to worry about not having proof of his debts and payments, nor about keeping track of them. Negative balances mean others owe him—the system handles it all.

## Getting Started

```powershell
# Compile
g++ main.cpp -o mott

# Run
.\mott
```

## Target Users

- Roommates sharing rent and utilities
- Friend groups with frequent shared expenses
- Family members managing household costs

## License

Academic project for IUT CSE4302 - Object-Oriented Programming
