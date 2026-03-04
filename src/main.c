#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "auth.h"
#include "product.h"
#include "cart.h"
#include "fraud.h"

bool is_numeric(const char *str) {
    if (str == NULL || *str == '\0') {
        return false;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return false;
        }
    }
    return true;
}

void process_request(const char* action, const char* param1, const char* param2) {
    product_init();
    auth_init();
    cart_init();
    fraud_init();

    printf("PROJECT: SmartEcom DSA Engine\n");
    printf("MODULE_CALL: %s\n", action);
    printf("--- DSA Trace ---\n");

    if (strcmp(action, "SEARCH") == 0) {
        const char* input = param1;
        Product* result = NULL;

        if (is_numeric(input)) {
            int id = atoi(input);
            result = product_search(id);

            if (result != NULL) {
                printf("RESULT: Product Found by ID (FAST LOOKUP). Name: %s, Price: %.2f\n", result->name, result->price);
                printf("DSA_USED: Product Catalog Lookup used Binary Search Tree (Conceptually AVL Tree).\n");
            } else {
                printf("RESULT: Product Not Found by ID %s.\n", input);
                printf("DSA_USED: Product Catalog Lookup failed.\n");
            }

        } else {
            result = product_search_by_name(input);

            if (result != NULL) {
                printf("RESULT: Product Found by Name. Name: %s, Price: %.2f\n", result->name, result->price);
                printf("DSA_USED: Name Search used full Catalog Traversal (slower than BST/AVL).\n");
            } else {
                printf("RESULT: Product Not Found by Name '%s'.\n", input);
                printf("DSA_USED: Name Search failed.\n");
            }
        }

    } else if (strcmp(action, "LOGIN") == 0) {
        if (auth_login_user(param1, param2)) {
            printf("AUTH_SUCCESS: Login Successful for user '%s'!\n", param1);
            printf("DSA_USED: Authentication used Hash Table for fast user lookup.\n");
        } else {
            printf("RESULT: Login Failed for user '%s'. Invalid credentials or user not found.\n", param1);
            printf("DSA_USED: Authentication used Hash Table lookup.\n");
        }

    } else if (strcmp(action, "REGISTER") == 0) {
        int new_id = 9999;
        auth_register_user(new_id, param1, param2);

        if (auth_login_user(param1, param2)) {
            printf("AUTH_SUCCESS: Registration Successful for user '%s'! Logging in...\n", param1);
            printf("DSA_USED: New user added to Hash Table.\n");
        }

    } else if (strcmp(action, "CART_ADD") == 0) {
        int id = atoi(param1);
        int qty = atoi(param2);
        cart_add_item(id, qty);

        printf("RESULT: Added %d x Product %d to cart.\n", qty, id);
        printf("DSA_USED: Cart used Linked List. Undo function state saved to Stack.\n");
        cart_display();

    } else if (strcmp(action, "CART_UNDO") == 0) {
        cart_undo_last_action();
        printf("DSA_USED: Undo function used Stack (LIFO) to reverse last cart operation.\n");
        cart_display();

    } else if (strcmp(action, "FRAUD_CHECK") == 0) {
        bool is_fraud = fraud_check_review(param1);

        if (is_fraud) {
            printf("FRAUD_ALERT: Review '%s' flagged as potentially fraudulent!\n", param1);
            printf("DSA_USED: Fraud detection module (Hash Table) detected duplicate content or banned phrase.\n");
        } else {
            printf("FRAUD_CHECK_OK: Review '%s' passed detection and was recorded.\n", param1);
            printf("DSA_USED: Fraud detection module (Hash Table) recorded the new review.\n");
        }

    } else if (strcmp(action, "DISPLAY_CATALOG") == 0) {
        product_display_all();
        printf("DSA_USED: Catalog displayed using In-order Traversal of the Binary Search Tree (BST/AVL Concept).\n");

    } else {
        printf("ERROR: Unknown action or missing parameters.\n");
    }

    printf("--- End of C Execution ---\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <ACTION> [PARAM1] [PARAM2]\n", argv[0]);
        return 1;
    }

    const char* param1 = (argc > 2) ? argv[2] : "";
    const char* param2 = (argc > 3) ? argv[3] : "";

    process_request(argv[1], param1, param2);

    return 0;
}
