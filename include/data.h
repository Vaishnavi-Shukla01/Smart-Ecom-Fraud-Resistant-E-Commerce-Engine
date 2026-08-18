#ifndef DATA_H
#define DATA_H

#define MAX_NAME_LEN 50
#define HASH_TABLE_SIZE 10
#define MAX_FILENAME_LEN 100

typedef struct User {
    int id;
    char username[MAX_NAME_LEN];
    char password_hash[MAX_NAME_LEN];
    struct User *next;
} User;

typedef struct Product {
    int id;
    char name[MAX_NAME_LEN];
    float price;
    struct Product *left;
    struct Product *right;
} Product;

typedef struct CartItem {
    int product_id;
    int quantity;
    struct CartItem *next;
} CartItem;

typedef struct UndoAction {
    int product_id;
    int quantity_change;
    struct UndoAction *next;
} UndoAction;

typedef struct Order {
    int order_id;
    char username[MAX_NAME_LEN];
    float total_amount;
    struct Order *next;
} Order;

#endif
