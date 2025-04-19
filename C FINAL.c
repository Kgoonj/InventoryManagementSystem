//Inventory-Management-System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 100
#define MAX_TRANSACTIONS 100
#define MAX_NAME_LENGTH 50
#define MAX_ID_LENGTH 20
#define MAX_TIMESTAMP_LENGTH 20

typedef struct {
    char id[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    int quantity;
    double price;
} Item;

typedef struct {
    char itemId[MAX_ID_LENGTH];
    int quantity;
    char type[4];
    char timestamp[MAX_TIMESTAMP_LENGTH];
} Transaction;

Item inventory[MAX_ITEMS];
Transaction transactions[MAX_TRANSACTIONS];
int inventoryCount = 0;
int transactionCount = 0;

void loadInventory(const char* filename);
void saveInventory(const char* filename);
void loadTransactions(const char* filename);
void saveTransactions(const char* filename);
void displayInventory();
void addItem();
void updateItem();
void deleteItem();
int findItemIndex(const char* id);
void recordTransaction(const char* itemId, int quantity, const char* type);
void displayTransactions();
void generateReports();
void searchInventory();
void sortInventory();
char* getCurrentTimestamp();
void deleteTransaction();
int compareById(const void* a, const void* b);
int compareByName(const void* a, const void* b);
int compareItems(const void* a, const void* b);



int main() {
    loadInventory("database.txt");
    printf("inventoryCount: %d\n", inventoryCount);
    loadTransactions("transactions.txt");

    int choice, quantity, i; // i declaration moved outside loops.
    char itemId[MAX_ID_LENGTH], type[4];

    do {
          printf("\t\t\t\t _________________________________________\n");
		    printf("\t\t\t\t|                                         |\n");
		    printf("\t\t\t\t|              Daily Shop                 |\n");
		    printf("\t\t\t\t|       Inventory Management System       |\n");
		    printf("\t\t\t\t|_________________________________________|\n");
		    printf("\t\t\t\t                                           \n");
		    printf("\t\t\t\t*******************************************\n");
		    printf("\t\t\t\t|        1. Display Inventory             |\n");
		    printf("\t\t\t\t|        2. Add Item                      |\n");
		    printf("\t\t\t\t|        3. Update Item                   |\n");
		    printf("\t\t\t\t|        4. Delete Item                   |\n");
		    printf("\t\t\t\t|        5. Record Transaction            |\n");
		    printf("\t\t\t\t|        6. Display Transactions          |\n");
		    printf("\t\t\t\t|        7. Generate Reports              |\n");
		    printf("\t\t\t\t|        8. Search Inventory              |\n");
		    printf("\t\t\t\t|        9. Sort Inventory                |\n");
		    printf("\t\t\t\t|        0. Exit                          |\n");
		    printf("\t\t\t\t*******************************************\n");
		    printf("\n\t\t\t\t Enter your choice: ");  
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                displayInventory();
                break;
            case 2:
                addItem();
                break;
            case 3:
                updateItem();
                break;
            case 4:
                deleteItem();
                break;
            case 5: {
                printf("Enter Item ID: ");
                scanf("%s", itemId);
                printf("Enter Quantity: ");
                scanf("%d", &quantity);
                printf("Enter Transaction Type (IN/OUT): ");
                scanf("%s", type);
                recordTransaction(itemId, quantity, type);
                break;
            }
            case 6:
                displayTransactions();
                break;
            case 7:
                generateReports();
                break;
            case 8:
                searchInventory();
                break;
            case 9:
                sortInventory();
                break;
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    saveInventory("database.txt");
    saveTransactions("transactions.txt");

    return 0;
}

void loadInventory(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open %s\n", filename);
        return;
    }

    inventoryCount = 0;
    printf("Reading inventory data...\n");

    while (fscanf(file, "%s %s %d %lf", 
                  inventory[inventoryCount].id, 
                  inventory[inventoryCount].name, 
                  &inventory[inventoryCount].quantity, 
                  &inventory[inventoryCount].price) == 4) {
        printf("Loaded: ID=%s, Name=%s, Quantity=%d, Price=%.2lf\n",
               inventory[inventoryCount].id, inventory[inventoryCount].name,
               inventory[inventoryCount].quantity, inventory[inventoryCount].price);
        inventoryCount++;
        if (inventoryCount >= MAX_ITEMS) break;
    }

    fclose(file);
    printf("Inventory loaded successfully. Total items: %d\n", inventoryCount);
}


void saveInventory(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file) {
        int i; // declaration moved outside loop
        for (i = 0; i < inventoryCount; i++) {
            fprintf(file, "%s %s %d %lf\n", inventory[i].id, inventory[i].name, inventory[i].quantity, inventory[i].price);
        }
        fclose(file);
    }
}

void loadTransactions(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        transactionCount = 0;
        int i; // declaration moved outside loop
        while (fscanf(file, "%[^,],%d,%[^,],%s", transactions[i].itemId, &transactions[i].quantity, transactions[i].type, transactions[i].timestamp) == 4) {
            transactionCount++;
            i++;
            if(transactionCount >= MAX_TRANSACTIONS) break;
        }
        fclose(file);
    }
}

void saveTransactions(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file) {
        int i; // declaration moved outside loop
        for (i = 0; i < transactionCount; i++) {
            fprintf(file, "%s,%d,%s,%s\n", transactions[i].itemId, transactions[i].quantity, transactions[i].type, transactions[i].timestamp);
        }
        fclose(file);
    }
}

void displayInventory() {
    printf("\nInventory:\n");
    int i; // declaration moved outside loop
    for (i = 0; i < inventoryCount; i++) {
        printf("ID: %s, Name: %s, Quantity: %d, Price: %.2lf\n", inventory[i].id, inventory[i].name, inventory[i].quantity, inventory[i].price);
    }
}

void addItem() {
    if (inventoryCount >= MAX_ITEMS) {
        printf("Inventory full.\n");
        return;
    }
    printf("Enter Item ID: ");
    scanf("%s", inventory[inventoryCount].id);
    printf("Enter Item Name: ");
    getchar();
    fgets(inventory[inventoryCount].name, MAX_NAME_LENGTH, stdin);
    inventory[inventoryCount].name[strcspn(inventory[inventoryCount].name, "\n")] = 0;
    printf("Enter Quantity: ");
    scanf("%d", &inventory[inventoryCount].quantity);
    printf("Enter Price: ");
    scanf("%lf", &inventory[inventoryCount].price);
    inventoryCount++;
    printf("Item added successfully!\n");
}

void updateItem() {
    char id[MAX_ID_LENGTH];
    printf("Enter Item ID to update: ");
    scanf("%s", id);
    int index = findItemIndex(id);
    if (index != -1) {
        printf("Enter new Quantity: ");
        scanf("%d", &inventory[index].quantity);
        printf("Enter new Price: ");
        scanf("%lf", &inventory[index].price);
        printf("Item updated successfully!\n");
    } else {
        printf("Item not found.\n");
    }
}

void deleteItem() {
    char id[MAX_ID_LENGTH];
    printf("Enter Item ID to delete: ");
    scanf("%s", id);
    int index = findItemIndex(id);
    if (index != -1) {
    	  int i;
        for (i = index; i < inventoryCount - 1; i++) {
            inventory[i] = inventory[i + 1];
        }
        inventoryCount--;
        printf("Item deleted successfully!\n");
    } else {
        printf("Item not found.\n");
    }
}


void displayTransactions() {
    printf("\nTransactions:\n");
    if (transactionCount == 0) {
        printf("No transactions recorded.\n");
        return;
    }
    int i;
    for (i = 0; i < transactionCount; i++) {
        printf("Item ID: %s | Quantity: %d | Type: %s | Timestamp: %s\n",
               transactions[i].itemId, transactions[i].quantity,
               transactions[i].type, transactions[i].timestamp);
    }
    printf("Transactions displayed successfully!\n");
}



int findItemIndex(const char* id) {
    int i; // declaration moved outside loop
    for (i = 0; i < inventoryCount; i++) {
        if (strcmp(inventory[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

void recordTransaction(const char* itemId, int quantity, const char* type) {
    char choice;
    
    do {
        if (transactionCount < MAX_TRANSACTIONS) {
            strcpy(transactions[transactionCount].itemId, itemId);
            transactions[transactionCount].quantity = quantity;
            strcpy(transactions[transactionCount].type, type);
            strcpy(transactions[transactionCount].timestamp, getCurrentTimestamp());
            transactionCount++;
            printf("Transaction recorded successfully!\n");
        } else {
            printf("Transaction log is full.\n");
            break;
        }

        printf("Do you want to add another transaction? (y/n): ");
        getchar(); // Consume newline character
        scanf("%c", &choice);

        if (choice == 'y' || choice == 'Y') {
            printf("Enter Item ID: ");
            scanf("%s", itemId);
            printf("Enter Quantity: ");
            scanf("%d", &quantity);
            printf("Enter Transaction Type (IN/OUT): ");
            scanf("%s", type);
        }

    } while (choice == 'y' || choice == 'Y');
}



void generateReports() {
    printf("\nReports:\n");
    printf("Total Items: %d\n", inventoryCount);
    
    printf("\nItems with Low Stock:\n");
    int i;
    for (i = 0; i < inventoryCount; i++) {
        if (inventory[i].quantity < 50) {
            printf("ID: %s, Name: %s, Quantity: %d\n", inventory[i].id, inventory[i].name, inventory[i].quantity);
        }
    }
    
    double totalInventoryValue = 0.0;
    for (i = 0; i < inventoryCount; i++) {
        totalInventoryValue += inventory[i].quantity * inventory[i].price;
    }
    printf("\nTotal Inventory Value: $%.2lf\n", totalInventoryValue);
}


void searchInventory() {
    char keyword[MAX_NAME_LENGTH];
    printf("Enter keyword to search: ");
    getchar();
    fgets(keyword, MAX_NAME_LENGTH, stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    printf("\nSearch Results:\n");
    int i; // declaration moved outside loop
    for (i = 0; i < inventoryCount; i++) {
        if (strstr(inventory[i].name, keyword) != NULL || strstr(inventory[i].id, keyword) != NULL) {
            printf("ID: %s, Name: %s, Quantity: %d, Price: %.2lf\n", inventory[i].id, inventory[i].name, inventory[i].quantity, inventory[i].price);
        }
    }
}

int compareItems(const void* a, const void* b) {
    return strcmp(((Item*)a)->name, ((Item*)b)->name);
}

int compareByName(const void* a, const void* b) {
    return strcmp(((Item*)a)->name, ((Item*)b)->name);
}

int compareById(const void* a, const void* b) {
    return strcmp(((Item*)a)->id, ((Item*)b)->id);
}

void sortInventory() {
    int choice;
    printf("Sort by:\n");
    printf("1. Name\n");
    printf("2. Item ID\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        qsort(inventory, inventoryCount, sizeof(Item), compareByName);
        printf("Inventory sorted by name.\n");
    } else if (choice == 2) {
        qsort(inventory, inventoryCount, sizeof(Item), compareById);
        printf("Inventory sorted by item ID.\n");
    } else {
        printf("Invalid choice. Sorting cancelled.\n");
    }
}


char* getCurrentTimestamp() {
    time_t now = time(0);
    struct tm tstruct;
    static char buf[MAX_TIMESTAMP_LENGTH];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);  // Ensure no newline added
    return buf;
}


