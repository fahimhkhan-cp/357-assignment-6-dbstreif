#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


char **split_string(const char *input, const char *delimiter, size_t *count) {
    char *temp = strdup(input);  // Duplicate the input string for modification
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed for input duplication.\n");
        return NULL;
    }

    size_t capacity = 10;       // Initial capacity for the list of substrings
    size_t index = 0;           // Index to keep track of the number of substrings
    char **result = malloc(capacity * sizeof(char *));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed for result array.\n");
        free(temp);
        return NULL;
    }

    char *token;
    char *rest = temp;

    while ((token = strsep(&rest, delimiter)) != NULL) {
        if (index >= capacity) {
            capacity *= 2;  // Double the capacity if needed
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (new_result == NULL) {
                fprintf(stderr, "Memory reallocation failed.\n");
                for (size_t i = 0; i < index; i++) free(result[i]); // Free allocated tokens
                free(result);
                free(temp);
                return NULL;
            }
            result = new_result;
        }
        result[index++] = strdup(token);  // Allocate memory for each substring
        if (result[index - 1] == NULL) {
            fprintf(stderr, "Memory allocation failed for token.\n");
            for (size_t i = 0; i < index - 1; i++) free(result[i]); // Free allocated tokens
            free(result);
            free(temp);
            return NULL;
        }
    }

    free(temp);  // Free the temporary duplicated string
    *count = index;  // Set the count to the number of substrings
    return result;
}


void free_string_list(char **list, size_t count) {
    for (size_t i = 0; i < count; i++) {
        free(list[i]);  // Free each substring
    }
    free(list);  // Free the list itself
}


void remove_quotes(char *str) {
    int i = 0, j = 0;

    // Iterate through the string and remove '"' characters
    while (str[i] != '\0') {
        if (str[i] != '"') {  // Skip quotation marks
            str[j++] = str[i]; // Copy character to the new position
        }
        i++;
    }
    str[j] = '\0';  // Null-terminate the modified string
}


int is_numeric(const char *str) {
    // Handle null or empty strings
    if (str == NULL || *str == '\0') {
        return 0;
    }

    // Allow a leading '+' or '-'
    if (*str == '+' || *str == '-') {
        str++;
    }

    int has_digits = 0; // Ensure at least one digit exists
    int has_decimal = 0; // Track if decimal point is allowed

    while (*str != '\0') {
        if (isdigit(*str)) {
            has_digits = 1;
        } else if (*str == '.' && !has_decimal) {
            has_decimal = 1; // Allow one decimal point
        } else {
            return 0; // Invalid character
        }
        str++;
    }

    return has_digits; // Valid numeric strings must have at least one digit
}
