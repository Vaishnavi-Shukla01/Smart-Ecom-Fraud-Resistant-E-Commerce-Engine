#ifndef FRAUD_H
#define FRAUD_H

#include "data.h"
#include <stdbool.h>

void fraud_init();
bool fraud_check_review(const char* new_review);

#endif
