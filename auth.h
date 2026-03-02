#ifndef AUTH_H
#define AUTH_H

#include "data.h"
#include <stdbool.h>

// Function Declarations
void auth_init();
void auth_register_user(int id, const char* username, const char* password);
bool auth_login_user(const char* username, const char* password);

#endif // AUTH_H