#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global Hash Table Array
User* user_hash_table[HASH_TABLE_SIZE];

// Simple Hash Function
static int user_hash(const char* username) {
    int sum = 0;
    for (int i = 0; username[i] != '\0'; i++) {
        sum = (sum + username[i]);
    }
    return sum % HASH_TABLE_SIZE;
}

// Helper: Checks if a user already exists in the Hash Table
static bool auth_user_exists(const char* username) {
    int index = user_hash(username);
    User* current = user_hash_table[index];
    
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            return true; // User found
        }
        current = current->next;
    }
    return false; // User not found
}

// Initialize the Hash Table
void auth_init() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        user_hash_table[i] = NULL;
    }
    // Pre-register some users for demo
    auth_register_user(1001, "teamlead", "1234");
    auth_register_user(1002, "vaishnavi", "5678");
}

// Register User (Add to Hash Table)
void auth_register_user(int id, const char* username, const char* password) {
    
    if (auth_user_exists(username)) {
        // Print ERROR message to console for Flask to capture
        printf("ERROR: Registration Failed. User '%s' already exists.\n", username);
        return;
    }

    int index = user_hash(username);
    
    User* newUser = (User*)malloc(sizeof(User));
    newUser->id = id;
    strcpy(newUser->username, username);
    strcpy(newUser->password_hash, password); // Simple copy for demo
    
    // Insert at the head of the linked list (Chaining)
    newUser->next = user_hash_table[index];
    user_hash_table[index] = newUser;
    
    // Success message is printed by main.c
}

// Login User (Lookup in Hash Table)
bool auth_login_user(const char* username, const char* password) {
    int index = user_hash(username);
    User* current = user_hash_table[index];
    
    while (current != NULL) {
        // Found matching username
        if (strcmp(current->username, username) == 0) {
            // Check password (In a real system, this checks the hash)
            if (strcmp(current->password_hash, password) == 0) {
                return true; // Login successful
            }
        }
        current = current->next;
    }
    return false; // Login failed
}