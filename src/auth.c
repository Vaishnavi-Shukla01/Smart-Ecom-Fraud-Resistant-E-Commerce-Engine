#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

User* user_hash_table[HASH_TABLE_SIZE];

static int user_hash(const char* username) {
    int sum = 0;
    for (int i = 0; username[i] != '\0'; i++) {
        sum = (sum + username[i]);
    }
    return sum % HASH_TABLE_SIZE;
}

static bool auth_user_exists(const char* username) {
    int index = user_hash(username);
    User* current = user_hash_table[index];

    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void auth_init() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        user_hash_table[i] = NULL;
    }
    auth_register_user(1001, "teamlead", "1234");
    auth_register_user(1002, "vaishnavi", "5678");
    auth_register_user(1003, "Aditya", "Aditya1234");
}

void auth_register_user(int id, const char* username, const char* password) {
    if (username == NULL || password == NULL) {
        printf("ERROR: Registration Failed. Invalid input.\n");
        return;
    }
    
    if (strlen(username) == 0 || strlen(username) >= MAX_NAME_LEN) {
        printf("ERROR: Registration Failed. Username length invalid.\n");
        return;
    }
    
    if (strlen(password) == 0 || strlen(password) >= MAX_NAME_LEN) {
        printf("ERROR: Registration Failed. Password length invalid.\n");
        return;
    }
    
    if (auth_user_exists(username)) {
        printf("ERROR: Registration Failed. User '%s' already exists.\n", username);
        return;
    }

    int index = user_hash(username);

    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for new User.\n");
        return;
    }
    
    newUser->id = id;
    
    strncpy(newUser->username, username, MAX_NAME_LEN - 1);
    newUser->username[MAX_NAME_LEN - 1] = '\0';
    
    strncpy(newUser->password_hash, password, MAX_NAME_LEN - 1);
    newUser->password_hash[MAX_NAME_LEN - 1] = '\0';

    newUser->next = user_hash_table[index];
    user_hash_table[index] = newUser;
}

bool auth_login_user(const char* username, const char* password) {
    if (username == NULL || password == NULL) {
        return false;
    }
    
    int index = user_hash(username);
    User* current = user_hash_table[index];

    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            if (strcmp(current->password_hash, password) == 0) {
                return true;
            }
        }
        current = current->next;
    }
    return false;
}
