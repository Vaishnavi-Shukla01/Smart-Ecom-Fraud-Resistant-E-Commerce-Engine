#include "cart.h"
#include "product.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir(dir, mode) _mkdir(dir)
#endif

#define CARTS_DIR "carts"

static CartItem* cart_head = NULL;
static UndoAction* undo_stack_head = NULL;
static char current_cart_filename[MAX_FILENAME_LEN] = {0};

static void free_current_state() {
    CartItem* current_cart = cart_head;
    while (current_cart != NULL) {
        CartItem* temp = current_cart;
        current_cart = current_cart->next;
        free(temp);
    }
    cart_head = NULL;

    UndoAction* current_undo = undo_stack_head;
    while (current_undo != NULL) {
        UndoAction* temp = current_undo;
        current_undo = current_undo->next;
        free(temp);
    }
    undo_stack_head = NULL;
}

static void ensure_carts_dir() {
    struct stat st = {0};
    if (stat(CARTS_DIR, &st) == -1) {
        mkdir(CARTS_DIR, 0700);
    }
}

static void cart_save() {
    if (current_cart_filename[0] == '\0') return;

    ensure_carts_dir();

    FILE *f = fopen(current_cart_filename, "w");
    if (f) {
        CartItem* current = cart_head;
        while (current != NULL) {
            if (current->quantity > 0) {
                fprintf(f, "%d|%d\n", current->product_id, current->quantity);
            }
            current = current->next;
        }
        fclose(f);
    } else {
        printf("ERROR: Could not save cart to %s.\n", current_cart_filename);
    }
}

static void push_undo(int id, int change) {
    UndoAction* newNode = (UndoAction*)malloc(sizeof(UndoAction));
    if (newNode == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for UndoAction.\n");
        return;
    }
    newNode->product_id = id;
    newNode->quantity_change = change;
    newNode->next = undo_stack_head;
    undo_stack_head = newNode;
}

static UndoAction* pop_undo() {
    if (undo_stack_head == NULL) return NULL;
    UndoAction* temp = undo_stack_head;
    undo_stack_head = undo_stack_head->next;
    return temp;
}

void cart_init() {
    free_current_state();
}

void cart_load(const char* username) {
    free_current_state();

    snprintf(current_cart_filename, MAX_FILENAME_LEN, "%s/%s.txt", CARTS_DIR, username);

    FILE *f = fopen(current_cart_filename, "r");
    if (!f) {
        printf("OK: Cart file for user '%s' not found. Starting with empty cart.\n", username);
        return;
    }

    char line[256];
    int id, qty;

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%d|%d", &id, &qty) == 2) {
            CartItem* newNode = (CartItem*)malloc(sizeof(CartItem));
            if (newNode) {
                newNode->product_id = id;
                newNode->quantity = qty;
                newNode->next = cart_head;
                cart_head = newNode;
            }
        }
    }
    fclose(f);
    printf("OK: Cart loaded for user '%s'.\n", username);
}

void cart_add_item(int product_id, int quantity) {
    if (current_cart_filename[0] == '\0') {
        printf("ERROR: Cannot add item. No user's cart is currently loaded.\n");
        return;
    }

    CartItem* current = cart_head;
    while (current != NULL) {
        if (current->product_id == product_id) {
            current->quantity += quantity;
            push_undo(product_id, -quantity);
            cart_save();
            return;
        }
        current = current->next;
    }

    CartItem* newNode = (CartItem*)malloc(sizeof(CartItem));
    if (newNode == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for new CartItem.\n");
        return;
    }
    newNode->product_id = product_id;
    newNode->quantity = quantity;
    newNode->next = cart_head;
    cart_head = newNode;

    push_undo(product_id, -quantity);
    cart_save();
}

void cart_remove_item(int product_id, int quantity) {
}

void cart_undo_last_action() {
    if (current_cart_filename[0] == '\0') {
        printf("ERROR: Cannot undo. No user's cart is currently loaded.\n");
        return;
    }

    UndoAction* action = pop_undo();
    if (action == NULL) {
        printf("UNDO_STATUS: No action to undo (Stack is empty).\n");
        return;
    }

    int reverse_change = action->quantity_change * -1;

    CartItem* current = cart_head;
    CartItem* prev = NULL;

    while (current != NULL) {
        if (current->product_id == action->product_id) {
            current->quantity += reverse_change;

            if (current->quantity <= 0) {
                printf("UNDO_STATUS: Removed Product %d from cart (Quantity reached 0).\n", action->product_id);
                if (prev == NULL) {
                    cart_head = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current);
            } else {
                printf("UNDO_STATUS: Successfully undid action for Product %d. New Quantity: %d.\n",
                       action->product_id, current->quantity);
            }

            free(action);
            cart_save();
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("UNDO_STATUS: Error: Product %d not found in cart during undo.\n", action->product_id);
    free(action);
}

void cart_display() {
    printf("--- Shopping Cart Contents (Linked List) ---\n");
    CartItem* current = cart_head;
    int count = 0;
    while (current != NULL) {
        if (current->quantity > 0) {
            printf("CART_ITEM:%d|%d\n", current->product_id, current->quantity);
        }
        current = current->next;
        count++;
    }
    if (count == 0) {
        printf("Cart is empty.\n");
    }
}

void cart_clear(const char* username) {
    snprintf(current_cart_filename, MAX_FILENAME_LEN, "%s/%s.txt", CARTS_DIR, username);

    free_current_state();

    remove(current_cart_filename);

    printf("RESULT: Cart contents cleared from memory and disk for %s.\n", username);
}

float cart_calculate_total() {
    float total = 0.0;
    CartItem* current = cart_head;

    while (current != NULL) {
        if (current->quantity > 0) {
            float item_price = product_get_price(current->product_id);
            total += (item_price * current->quantity);
        }
        current = current->next;
    }
    return total;
}
