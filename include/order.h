#ifndef ORDER_H
#define ORDER_H

#include "data.h"
#include <stdbool.h>

void order_init();
void order_process_new(const char* username, float total_amount);
void order_display_all();
void order_clear_queue();

#endif
