#ifndef HELPERS_H_
#define HELPERS_H_

#include <stddef.h>

char **split_string(const char *input, const char *delimeter, size_t *count);
void free_string_list(char **list, size_t count);
void remove_quotes(char *str);
int is_numeric(const char *str);

#endif  // HELPERS_H_
