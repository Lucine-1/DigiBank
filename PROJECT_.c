#include <stdio.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 1000
#define DAILY_LIMIT 10000.0

struct Account
{
    int accountNumber;
    char name[50];
    int pin;
    double balance;
    int active;
    double dailyWithdrawn;
};

struct Transaction
{
    int transactionID;
    int accountNumber;
    int receiverAccount;
    char type[20];
    double amount;
};

struct Account accounts[MAX_ACCOUNTS];
struct Transaction transactions[MAX_TRANSACTIONS];

int accountCount = 0;
int transactionCount = 0;
int nextAccountNumber = 1001;
int nextTransactionID = 1;

/* Function declarations */

int findAccountIndex(int accNo);
int getFreeAccountIndex(void);
void recordTransaction(int accNo, int receiver, char type[], double amount);
void createAccount(void);
void searchAccount(void);
void displayAllAccounts(void);
void deleteAccount(void);
void authenticateUser(void);
void checkBalance(int index);
void depositMoney(int index);
void withdrawMoney(int index);
void transferMoney(int index);
void changePIN(int index);
void showTransactionHistory(int accNo);
void showMiniStatement(int accNo);
void displayStatistics(void);
void atmMenu(int index);
void mainMenu(void);

/* Find an active account */

int findAccountIndex(int accNo)
{
    int i;

    for (i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].active == 1 &&
            accounts[i].accountNumber == accNo)
        {
            return i;
        }
    }

    return -1;
}

/* Find an available array position */

int getFreeAccountIndex(void)
{
    int i;

    for (i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].active == 0)
        {
            return i;
        }
    }

    return -1;
}

/* Record transactions */

void recordTransaction(int accNo, int receiver, char type[], double amount)
{
    struct Transaction *t;

    if (transactionCount >= MAX_TRANSACTIONS)
    {
        printf("Transaction history is full.\n");
        return;
    }

    t = &transactions[transactionCount];

    t->transactionID = nextTransactionID++;
    t->accountNumber = accNo;
    t->receiverAccount = receiver;
    strcpy(t->type, type);
    t->amount = amount;

    transactionCount++;
}

/* Create account */

void createAccount(void)
{
    int index;
    int pin;
    char name[50];

    index = getFreeAccountIndex();

    if (index == -1)
    {
        printf("\nBank account storage is full!\n");
        return;
    }

    printf("\nEnter customer name: ");
    scanf(" %49[^\n]", name);

    printf("Create a 4-digit PIN: ");
    if (scanf("%d", &pin) != 1)
    {
        printf("Invalid PIN input!\n");
        return;
    }

    if (pin < 1000 || pin > 9999)
    {
        printf("PIN must contain exactly 4 digits.\n");
        return;
    }

    accounts[index].accountNumber = nextAccountNumber++;
    strcpy(accounts[index].name, name);
    accounts[index].pin = pin;
    accounts[index].balance = 0;
    accounts[index].dailyWithdrawn = 0;
    accounts[index].active = 1;

    accountCount++;

    printf("\nAccount created successfully!\n");
    printf("Account Holder: %s\n", accounts[index].name);
    printf("Account Number: %d\n", accounts[index].accountNumber);
}

/* Search account */

void searchAccount(void)
{
    int accNo;
    int index;

    printf("\nEnter account number: ");
    if (scanf("%d", &accNo) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    index = findAccountIndex(accNo);

    if (index == -1)
    {
        printf("Account not found!\n");
        return;
    }

    printf("\nAccount Holder: %s\n", accounts[index].name);
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Balance: Rs. %.2lf\n", accounts[index].balance);
}

/* Display all accounts */

void displayAllAccounts(void)
{
    int i;
    int found = 0;

    printf("\n----- ALL ACTIVE ACCOUNTS -----\n");

    for (i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].active == 1)
        {
            printf("\nAccount Number: %d\n",
                   accounts[i].accountNumber);

            printf("Name: %s\n", accounts[i].name);
            printf("Balance: Rs. %.2lf\n", accounts[i].balance);

            found = 1;
        }
    }

    if (!found)
    {
        printf("No active accounts found.\n");
    }
}

/* Delete account */

void deleteAccount(void)
{
    int accNo;
    int index;
    int confirm;

    printf("\nEnter account number to delete: ");

    if (scanf("%d", &accNo) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    index = findAccountIndex(accNo);

    if (index == -1)
    {
        printf("Account not found!\n");
        return;
    }

    if (accounts[index].balance != 0)
    {
        printf("\nCannot delete account!\n");
        printf("Account balance must be zero.\n");
        return;
    }

    printf("\nAccount Holder: %s\n", accounts[index].name);
    printf("Account Number: %d\n", accounts[index].accountNumber);

    printf("\nAre you sure you want to delete this account?\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf("Enter choice: ");

    if (scanf("%d", &confirm) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    if (confirm == 1)
    {
        accounts[index].active = 0;
        accounts[index].pin = 0;
        strcpy(accounts[index].name, "");

        accountCount--;

        printf("\nAccount deleted successfully!\n");
    }
    else
    {
        printf("\nAccount deletion cancelled.\n");
    }
}

/* Authenticate user */

void authenticateUser(void)
{
    int accNo;
    int pin;
    int index;
    int attempts = 0;

    printf("\nEnter account number: ");

    if (scanf("%d", &accNo) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    index = findAccountIndex(accNo);

    if (index == -1)
    {
        printf("Account not found!\n");
        return;
    }

    while (attempts < 3)
    {
        printf("Enter PIN: ");

        if (scanf("%d", &pin) != 1)
        {
            printf("Invalid input!\n");
            return;
        }

        if (pin == accounts[index].pin)
        {
            printf("\nLogin successful!\n");
            atmMenu(index);
            return;
        }

        attempts++;
        printf("Incorrect PIN! Attempts remaining: %d\n",
               3 - attempts);
    }

    printf("\nToo many incorrect attempts.\n");
}

/* Check balance */

void checkBalance(int index)
{
    printf("\nAvailable Balance: Rs. %.2lf\n",
           accounts[index].balance);
}

/* Deposit money */

void depositMoney(int index)
{
    double amount;

    if (transactionCount >= MAX_TRANSACTIONS)
    {
        printf("Transaction history is full.\n");
        return;
    }

    printf("\nEnter deposit amount: Rs. ");

    if (scanf("%lf", &amount) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    if (amount <= 0)
    {
        printf("Enter a valid amount!\n");
        return;
    }

    accounts[index].balance += amount;

    recordTransaction(
        accounts[index].accountNumber,
        0,
        "DEPOSIT",
        amount
    );

    printf("\nDeposit successful!\n");
    checkBalance(index);
}

/* Withdraw money */

void withdrawMoney(int index)
{
    double amount;

    if (transactionCount >= MAX_TRANSACTIONS)
    {
        printf("Transaction history is full.\n");
        return;
    }

    printf("\nEnter withdrawal amount: Rs. ");

    if (scanf("%lf", &amount) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    if (amount <= 0)
    {
        printf("Invalid amount!\n");
        return;
    }

    if (amount > accounts[index].balance)
    {
        printf("Insufficient balance!\n");
        return;
    }

    if (accounts[index].dailyWithdrawn + amount > DAILY_LIMIT)
    {
        printf("Daily withdrawal limit exceeded!\n");
        return;
    }

    accounts[index].balance -= amount;
    accounts[index].dailyWithdrawn += amount;

    recordTransaction(
        accounts[index].accountNumber,
        0,
        "WITHDRAW",
        amount
    );

    printf("\nWithdrawal successful!\n");
    checkBalance(index);
}

/* Transfer money */

void transferMoney(int index)
{
    int receiverAcc;
    int receiverIndex;
    double amount;

    if (transactionCount + 2 > MAX_TRANSACTIONS)
    {
        printf("Not enough transaction history space.\n");
        return;
    }

    printf("\nEnter receiver account number: ");

    if (scanf("%d", &receiverAcc) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    receiverIndex = findAccountIndex(receiverAcc);

    if (receiverIndex == -1)
    {
        printf("Receiver account not found!\n");
        return;
    }

    if (receiverIndex == index)
    {
        printf("You cannot transfer money to your own account!\n");
        return;
    }

    printf("Enter amount to transfer: Rs. ");

    if (scanf("%lf", &amount) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    if (amount <= 0)
    {
        printf("Invalid amount!\n");
        return;
    }

    if (amount > accounts[index].balance)
    {
        printf("Insufficient balance!\n");
        return;
    }

    accounts[index].balance -= amount;
    accounts[receiverIndex].balance += amount;

    recordTransaction(
        accounts[index].accountNumber,
        receiverAcc,
        "TRANSFER OUT",
        amount
    );

    recordTransaction(
        receiverAcc,
        accounts[index].accountNumber,
        "TRANSFER IN",
        amount
    );

    printf("\nTransfer successful!\n");
    checkBalance(index);
}

/* Change PIN */

void changePIN(int index)
{
    int oldPIN;
    int newPIN;

    printf("\nEnter old PIN: ");

    if (scanf("%d", &oldPIN) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    if (oldPIN != accounts[index].pin)
    {
        printf("Incorrect old PIN!\n");
        return;
    }

    printf("Enter new 4-digit PIN: ");

    if (scanf("%d", &newPIN) != 1)
    {
        printf("Invalid input!\n");
        return;
    }

    if (newPIN < 1000 || newPIN > 9999)
    {
        printf("PIN must contain exactly 4 digits.\n");
        return;
    }

    accounts[index].pin = newPIN;

    printf("\nPIN changed successfully!\n");
}

/* Transaction history */

void showTransactionHistory(int accNo)
{
    int i;
    int found = 0;

    printf("\n----- TRANSACTION HISTORY -----\n");

    for (i = 0; i < transactionCount; i++)
    {
        if (transactions[i].accountNumber == accNo)
        {
            printf("\nTransaction ID: %d\n",
                   transactions[i].transactionID);

            printf("Type: %s\n", transactions[i].type);
            printf("Amount: Rs. %.2lf\n",
                   transactions[i].amount);

            if (transactions[i].receiverAccount != 0)
            {
                printf("Other Account: %d\n",
                       transactions[i].receiverAccount);
            }

            found = 1;
        }
    }

    if (!found)
    {
        printf("No transactions found.\n");
    }
}

/* Mini statement */

void showMiniStatement(int accNo)
{
    int i;
    int count = 0;

    printf("\n----- MINI STATEMENT -----\n");

    for (i = transactionCount - 1; i >= 0 && count < 5; i--)
    {
        if (transactions[i].accountNumber == accNo)
        {
            printf("\n%s : Rs. %.2lf\n",
                   transactions[i].type,
                   transactions[i].amount);

            count++;
        }
    }

    if (count == 0)
    {
        printf("No transactions available.\n");
    }
}

/* Banking statistics */

void displayStatistics(void)
{
    int i;
    int totalActive = 0;
    double totalBalance = 0;

    for (i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].active == 1)
        {
            totalActive++;
            totalBalance += accounts[i].balance;
        }
    }

    printf("\n----- BANK STATISTICS -----\n");
    printf("Total Active Accounts: %d\n", totalActive);
    printf("Total Bank Balance: Rs. %.2lf\n", totalBalance);
    printf("Total Transactions: %d\n", transactionCount);
}

/* ATM menu */

void atmMenu(int index)
{
    int choice;

    do
    {
        printf("\n========== ATM MENU ==========\n");
        printf("1. Check Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer Money\n");
        printf("5. Change PIN\n");
        printf("6. Mini Statement\n");
        printf("7. Transaction History\n");
        printf("0. Logout\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            return;
        }

        switch (choice)
        {
            case 1:
                checkBalance(index);
                break;

            case 2:
                depositMoney(index);
                break;

            case 3:
                withdrawMoney(index);
                break;

            case 4:
                transferMoney(index);
                break;

            case 5:
                changePIN(index);
                break;

            case 6:
                showMiniStatement(accounts[index].accountNumber);
                break;

            case 7:
                showTransactionHistory(accounts[index].accountNumber);
                break;

            case 0:
                printf("\nLogged out successfully!\n");
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while (choice != 0);
}

/* Main banking menu */

void mainMenu(void)
{
    int choice;

    do
    {
        printf("\n========== MINI BANKING SYSTEM ==========\n");
        printf("1. Create Bank Account\n");
        printf("2. Search Account\n");
        printf("3. Display All Accounts\n");
        printf("4. ATM Login\n");
        printf("5. Banking Statistics\n");
        printf("6. Exit\n");
        printf("7. Delete Bank Account\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            return;
        }

        switch (choice)
        {
            case 1:
                createAccount();
                break;

            case 2:
                searchAccount();
                break;

            case 3:
                displayAllAccounts();
                break;

            case 4:
                authenticateUser();
                break;

            case 5:
                displayStatistics();
                break;

            case 6:
                printf("\nThank you for using our banking system!\n");
                break;

            case 7:
                deleteAccount();
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while (choice != 6);
}

/* Main function */

int main(void)
{
    mainMenu();

    return 0;
}
