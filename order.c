#include "order.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDERS_FILE "orders.txt"

// Global Queue Pointers
// FRONT: Where orders are dequeued (processed) - we use this only for display
// REAR: Where new orders are enqueued (added)
static Order *order_front = NULL;
static Order *order_rear = NULL;
static int next_order_id = 1;

// --- Order Persistence (Loading & Saving) ---

// Helper to save all orders currently in the queue to file
static void order_save() {
    FILE *f = fopen(ORDERS_FILE, "w");
    if (!f) {
        printf("ERROR: Could not save orders to %s.\n", ORDERS_FILE);
        return;
    }

    Order* current = order_front;
    while (current != NULL) {
        // Save in simple format: id|user|total\n
        fprintf(f, "%d|%s|%.2f\n", current->order_id, current->username, current->total_amount);
        current = current->next;
    }
    fclose(f);
}

// Helper to add an order node (ENQUEUE operation)
static void enqueue_order(int id, const char* username, float total) {
    Order* newOrder = (Order*)malloc(sizeof(Order));
    if (newOrder == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for Order node.\n");
        return;
    }
    
    newOrder->order_id = id;
    strncpy(newOrder->username, username, MAX_NAME_LEN - 1);
    newOrder->total_amount = total;
    newOrder->next = NULL;

    if (order_rear == NULL) {
        // If queue is empty
        order_front = newOrder;
        order_rear = newOrder;
    } else {
        // Add new order to the rear
        order_rear->next = newOrder;
        order_rear = newOrder;
    }
}

// Initialize Order Queue (Loads existing orders)
void order_init() {
    order_front = NULL;
    order_rear = NULL;
    next_order_id = 1;

    FILE *f = fopen(ORDERS_FILE, "r");
    if (f) {
        char line[512];
        int id;
        char username[MAX_NAME_LEN];
        float total;

        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d|%49[^|]|%f", &id, username, &total) == 3) {
                // Load existing orders back into the queue
                enqueue_order(id, username, total);
                if (id >= next_order_id) {
                    next_order_id = id + 1;
                }
            }
        }
        fclose(f);
        printf("OK: Order History (Queue) loaded from %s. Next ID: %d.\n", ORDERS_FILE, next_order_id);
    } else {
        printf("WARNING: %s file not found. Starting with empty Order Queue.\n", ORDERS_FILE);
    }
}

// Process a new order and add it to the queue
void order_process_new(const char* username, float total_amount) {
    printf("MODULE_CALL: Order Processor (ENQUEUE)\n");
    
    int new_id = next_order_id++;
    
    // 1. ENQUEUE the new order
    enqueue_order(new_id, username, total_amount);
    
    // 2. Save the updated queue to disk
    order_save();

    printf("RESULT: Order #%d processed and added to Queue for fulfillment.\n", new_id);
    printf("DSA_USED: Order used Queue (FIFO) for processing sequence, saved to disk.\n");
}

// Display all orders currently in the queue
void order_display_all() {
    printf("--- Order Processing Queue (FIFO) ---\n");
    Order* current = order_front;
    int count = 0;
    while (current != NULL) {
        // Structured output for Flask
        printf("ORDER_ITEM:%d|%s|%.2f\n", current->order_id, current->username, current->total_amount);
        current = current->next;
        count++;
    }
    if (count == 0) {
        printf("Order Queue is empty.\n");
    }
}

// For cleanup/debugging
void order_clear_queue() {
    Order* current = order_front;
    while (current != NULL) {
        Order* temp = current;
        current = current->next;
        free(temp);
    }
    order_front = NULL;
    order_rear = NULL;
    remove(ORDERS_FILE); // Delete the file for a clean start
}