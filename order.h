#ifndef ORDER_H
#define ORDER_H

#include "data.h"
#include <stdbool.h>

// Function Declarations
void order_init();
void order_process_new(const char* username, float total_amount);
void order_display_all();
void order_clear_queue(); // For debugging/testing

#endif // ORDER_H