#include "product.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Product* catalog_root = NULL;

static Product* create_product_node(int id, const char* name, float price) {
    Product* newNode = (Product*)malloc(sizeof(Product));
    newNode->id = id;
    strcpy(newNode->name, name);
    newNode->price = price;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

static Product* insert_product_bst(Product* root, Product* newProd) {
    if (root == NULL) return newProd;

    if (newProd->id < root->id) {
        root->left = insert_product_bst(root->left, newProd);
    } else if (newProd->id > root->id) {
        root->right = insert_product_bst(root->right, newProd);
    }
    return root;
}

void product_init() {
    if (catalog_root == NULL) {
        catalog_root = insert_product_bst(catalog_root, create_product_node(101, "DSA Textbook", 850.00));
        catalog_root = insert_product_bst(catalog_root, create_product_node(505, "Monitor 4K", 28000.00));
        catalog_root = insert_product_bst(catalog_root, create_product_node(202, "Webcam HD", 1500.00));
        catalog_root = insert_product_bst(catalog_root, create_product_node(303, "Wireless Mouse", 450.00));
    }
}

Product* product_search(int id) {
    Product* current = catalog_root;
    while (current != NULL) {
        if (id == current->id) {
            return current;
        } else if (id < current->id) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

Product* recursive_name_search(Product* root, const char* target_name) {
    if (root == NULL) {
        return NULL;
    }

    char root_name_lower[MAX_NAME_LEN];
    char target_lower[MAX_NAME_LEN];
    size_t len_root = strlen(root->name);
    size_t len_target = strlen(target_name);

    if (len_root >= MAX_NAME_LEN || len_target >= MAX_NAME_LEN) {
        return NULL;
    }

    for (size_t i = 0; i <= len_root; i++) root_name_lower[i] = tolower(root->name[i]);
    for (size_t i = 0; i <= len_target; i++) target_lower[i] = tolower(target_name[i]);

    if (strstr(root_name_lower, target_lower) != NULL) {
        return root;
    }

    Product* found_in_left = recursive_name_search(root->left, target_name);
    if (found_in_left != NULL) {
        return found_in_left;
    }

    return recursive_name_search(root->right, target_name);
}

Product* product_search_by_name(const char* name) {
    return recursive_name_search(catalog_root, name);
}

float product_get_price(int id) {
    Product* prod = product_search(id);
    if (prod != NULL) {
        return prod->price;
    }
    return 0.0f;
}

void product_display_inorder(Product* root) {
    if (root != NULL) {
        product_display_inorder(root->left);
        printf("ID: %d, Name: %s, Price: %.2f\n", root->id, root->name, root->price);
        product_display_inorder(root->right);
    }
}

void product_display_all() {
    printf("--- Current Product Catalog (BST In-Order Traversal) ---\n");
    product_display_inorder(catalog_root);
}
