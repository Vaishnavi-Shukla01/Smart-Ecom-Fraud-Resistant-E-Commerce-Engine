#ifndef DATA_H
#define DATA_H

// --- Shared Constants ---
#define MAX_NAME_LEN 50
#define HASH_TABLE_SIZE 10
#define MAX_FILENAME_LEN 100 // New constant for file paths

// --- User Structure (Used in Hash Table for Auth) ---
typedef struct User {
    int id;
    char username[MAX_NAME_LEN];
    char password_hash[MAX_NAME_LEN]; // Storing a simple 'hash' (password itself for simplicity)
    struct User *next; // For chaining in the Hash Table
} User;

// --- Product Structure (Used in AVL Tree for Catalog) ---
typedef struct Product {
    int id;
    char name[MAX_NAME_LEN];
    float price;
    struct Product *left;
    struct Product *right;
} Product;

// --- Cart Item Structure (Used in Linked List for Cart) ---
typedef struct CartItem {
    int product_id;
    int quantity;
    struct CartItem *next;
} CartItem;

// --- Stack Node for Cart Undo (Used in Stack) ---
typedef struct UndoAction {
    int product_id;
    int quantity_change; // + for add, - for remove
    struct UndoAction *next;
} UndoAction;

// --- Order Structure (Used in Queue for Processing/History) ---
typedef struct Order {
    int order_id;
    char username[MAX_NAME_LEN];
    float total_amount;
    // We will not store items here for simplicity, just the summary.
    struct Order *next;
} Order;


#endif // DATA_H