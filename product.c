#include "product.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For tolower()

// Global root for the Product Catalog (Conceptually an AVL Tree)
static Product* catalog_root = NULL;

// Function to create a new product node
static Product* create_product_node(int id, const char* name, float price) {
    Product* newNode = (Product*)malloc(sizeof(Product));
    newNode->id = id;
    strcpy(newNode->name, name);
    newNode->price = price;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Simple BST insert (simulating the AVL Tree structure for fast search)
static Product* insert_product_bst(Product* root, Product* newProd) {
    if (root == NULL) return newProd;
    
    if (newProd->id < root->id) {
        root->left = insert_product_bst(root->left, newProd);
    } else if (newProd->id > root->id) {
        root->right = insert_product_bst(root->right, newProd);
    }
    return root;
}

// Initialize Product Catalog 
void product_init() {
    // Only initialize if not already done
    if (catalog_root == NULL) {
        catalog_root = insert_product_bst(catalog_root, create_product_node(101, "DSA Textbook", 850.00));
        catalog_root = insert_product_bst(catalog_root, create_product_node(505, "Monitor 4K", 28000.00));
        catalog_root = insert_product_bst(catalog_root, create_product_node(202, "Webcam HD", 1500.00));
        catalog_root = insert_product_bst(catalog_root, create_product_node(303, "Wireless Mouse", 450.00));
    }
}

// Search by ID (Fast Product Search - BST/AVL lookup)
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

// Recursive Name Search (Simulates sequential or less efficient traversal)
Product* recursive_name_search(Product* root, const char* target_name) {
    if (root == NULL) {
        return NULL;
    }

    // Convert product name and target name to lowercase for case-insensitive search
    char root_name_lower[MAX_NAME_LEN];
    char target_lower[MAX_NAME_LEN];
    size_t len_root = strlen(root->name);
    size_t len_target = strlen(target_name);

    if (len_root >= MAX_NAME_LEN || len_target >= MAX_NAME_LEN) {
        // Handle error or skip if name is too long
        return NULL;
    }

    for (size_t i = 0; i <= len_root; i++) root_name_lower[i] = tolower(root->name[i]);
    for (size_t i = 0; i <= len_target; i++) target_lower[i] = tolower(target_name[i]);

    if (strstr(root_name_lower, target_lower) != NULL) {
        return root; // Found a match (sub-string match)
    }

    // Continue search in both subtrees (this is an inefficient search!)
    Product* found_in_left = recursive_name_search(root->left, target_name);
    if (found_in_left != NULL) {
        return found_in_left;
    }

    return recursive_name_search(root->right, target_name);
}

// Search by Name (Public function)
Product* product_search_by_name(const char* name) {
    return recursive_name_search(catalog_root, name);
}


// Simple in-order traversal to display (for internal testing/demo)
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