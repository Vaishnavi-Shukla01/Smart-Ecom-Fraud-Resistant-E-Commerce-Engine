#ifndef PRODUCT_H
#define PRODUCT_H

#include "data.h"
#include <stdbool.h>

// Function Declarations
void product_init();
Product* product_search(int id);
Product* product_search_by_name(const char* name);
void product_display_all();

#endif // PRODUCT_H