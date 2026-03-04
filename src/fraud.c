#include "fraud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REVIEW_HASH_SIZE 5
#define MAX_REVIEW_LEN 100

typedef struct ReviewEntry {
    char review_content[MAX_REVIEW_LEN];
    struct ReviewEntry *next;
} ReviewEntry;

static ReviewEntry* review_hash_table[REVIEW_HASH_SIZE];

static int simple_string_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % REVIEW_HASH_SIZE;
}

void fraud_init() {
    for (int i = 0; i < REVIEW_HASH_SIZE; i++) {
        review_hash_table[i] = NULL;
    }
    fraud_check_review("Excellent quality, highly recommend.");
    fraud_check_review("Fast delivery, good packaging.");
}

bool fraud_check_review(const char* new_review) {
    if (new_review == NULL || strlen(new_review) == 0) {
        return false;
    }
    
    int index = simple_string_hash(new_review);
    ReviewEntry* current = review_hash_table[index];

    while (current != NULL) {
        if (strcmp(current->review_content, new_review) == 0) {
            return true;
        }
        current = current->next;
    }

    if (strstr(new_review, "fake") || strstr(new_review, "scam") || strstr(new_review, "fraud")) {
        return true;
    }

    ReviewEntry* newEntry = (ReviewEntry*)malloc(sizeof(ReviewEntry));
    if (newEntry == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for ReviewEntry.\n");
        return false;
    }
    
    strncpy(newEntry->review_content, new_review, MAX_REVIEW_LEN - 1);
    newEntry->review_content[MAX_REVIEW_LEN - 1] = '\0';
    
    newEntry->next = review_hash_table[index];
    review_hash_table[index] = newEntry;

    return false;
}
