#ifndef CART_H
#define CART_H

#include "data.h"
#include <stdbool.h>

// Function Declarations
void cart_init();
void cart_load(const char* username); 
void cart_add_item(int product_id, int quantity);
void cart_remove_item(int product_id, int quantity);
void cart_undo_last_action();
void cart_display();
void cart_clear(const char* username); // Function to clear cart
float cart_calculate_total(); // Function to calculate total amount

#endif // CART_H