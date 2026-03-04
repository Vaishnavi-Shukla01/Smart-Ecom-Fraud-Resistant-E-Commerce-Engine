#ifndef CART_H
#define CART_H

#include "data.h"
#include <stdbool.h>

void cart_init();
void cart_load(const char* username);
void cart_add_item(int product_id, int quantity);
void cart_remove_item(int product_id, int quantity);
void cart_undo_last_action();
void cart_display();
void cart_clear(const char* username);
float cart_calculate_total();

#endif
