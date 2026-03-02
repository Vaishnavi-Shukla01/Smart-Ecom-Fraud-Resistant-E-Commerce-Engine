#include "fraud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Review Hash Table Structure for Fraud Detection ---
// We use a simple Hash Table array for demonstration
#define REVIEW_HASH_SIZE 5
typedef struct ReviewEntry {
    char review_content[100];
    struct ReviewEntry *next;
} ReviewEntry;

static ReviewEntry* review_hash_table[REVIEW_HASH_SIZE];

// Simple String Hashing Function (for demonstration)
static int simple_string_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % REVIEW_HASH_SIZE;
}

// Initialize the Hash Table
void fraud_init() {
    for (int i = 0; i < REVIEW_HASH_SIZE; i++) {
        review_hash_table[i] = NULL;
    }
    // Pre-populate with some reviews
    // These reviews will be flagged as duplicates if entered by the user
    // Note: We intentionally call fraud_check_review here to register the initial "trusted" reviews
    fraud_check_review("Excellent quality, highly recommend.");
    fraud_check_review("Fast delivery, good packaging.");
}

// Check for duplicate review (Fraud Detection Concept)
// Returns true if duplicate or banned phrase found, false otherwise
bool fraud_check_review(const char* new_review) {
    int index = simple_string_hash(new_review);
    ReviewEntry* current = review_hash_table[index];

    // 1. Check for Duplicate Review (Hash Table Lookup)
    while (current != NULL) {
        if (strcmp(current->review_content, new_review) == 0) {
            return true; // Duplicate (Fraudulent) found
        }
        current = current->next;
    }

    // 2. Simple Banned Phrase Check
    if (strstr(new_review, "fake") || strstr(new_review, "scam") || strstr(new_review, "fraud")) {
        return true; // Contains banned phrase
    }

    // 3. If passed checks, add the review to the hash table for future checks
    ReviewEntry* newEntry = (ReviewEntry*)malloc(sizeof(ReviewEntry));
    if (newEntry == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for ReviewEntry.\n");
        return false;
    }
    strcpy(newEntry->review_content, new_review);
    newEntry->next = review_hash_table[index];
    review_hash_table[index] = newEntry;

    return false; // No fraud detected
}