#include "order.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDERS_FILE "orders.txt"

static Order *order_front = NULL;
static Order *order_rear = NULL;
static int next_order_id = 1;

static void order_save() {
    FILE *f = fopen(ORDERS_FILE, "w");
    if (!f) {
        printf("ERROR: Could not save orders to %s.\n", ORDERS_FILE);
        return;
    }

    Order* current = order_front;
    while (current != NULL) {
        fprintf(f, "%d|%s|%.2f\n", current->order_id, current->username, current->total_amount);
        current = current->next;
    }
    fclose(f);
}

static void enqueue_order(int id, const char* username, float total) {
    if (username == NULL) {
        fprintf(stderr, "ERROR: Invalid username for order.\n");
        return;
    }
    
    Order* newOrder = (Order*)malloc(sizeof(Order));
    if (newOrder == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed for Order node.\n");
        return;
    }

    newOrder->order_id = id;
    
    strncpy(newOrder->username, username, MAX_NAME_LEN - 1);
    newOrder->username[MAX_NAME_LEN - 1] = '\0';
    
    newOrder->total_amount = total;
    newOrder->next = NULL;

    if (order_rear == NULL) {
        order_front = newOrder;
        order_rear = newOrder;
    } else {
        order_rear->next = newOrder;
        order_rear = newOrder;
    }
}

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
            memset(username, 0, sizeof(username));
            if (sscanf(line, "%d|%49[^|]|%f", &id, username, &total) == 3) {
                username[MAX_NAME_LEN - 1] = '\0';
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

void order_process_new(const char* username, float total_amount) {
    if (username == NULL) {
        printf("ERROR: Cannot process order. Invalid username.\n");
        return;
    }
    
    printf("MODULE_CALL: Order Processor (ENQUEUE)\n");

    int new_id = next_order_id++;

    enqueue_order(new_id, username, total_amount);

    order_save();

    printf("RESULT: Order #%d processed and added to Queue for fulfillment.\n", new_id);
    printf("DSA_USED: Order used Queue (FIFO) for processing sequence, saved to disk.\n");
}

void order_display_all() {
    printf("--- Order Processing Queue (FIFO) ---\n");
    Order* current = order_front;
    int count = 0;
    while (current != NULL) {
        printf("ORDER_ITEM:%d|%s|%.2f\n", current->order_id, current->username, current->total_amount);
        current = current->next;
        count++;
    }
    if (count == 0) {
        printf("Order Queue is empty.\n");
    }
}

void order_clear_queue() {
    Order* current = order_front;
    while (current != NULL) {
        Order* temp = current;
        current = current->next;
        free(temp);
    }
    order_front = NULL;
    order_rear = NULL;
    remove(ORDERS_FILE);
}
