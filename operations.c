#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "corgis_struct.h"
#include "operations.h"

const char *fields[] = {
    "County",                                      // a string
    "State",                                       // a string
    "Education.Bachelor's Degree or Higher",       // float
    "Education.High School or Higher",             // float
    "Ethnicities.American Indian and Alaska Native Alone",  // float
    "Ethnicities.Asian Alone",                     // float
    "Ethnicities.Black Alone",                     // float
    "Ethnicities.Hispanic or Latino",              // float
    "Ethnicities.Native Hawaiian and Other Pacific Islander Alone",  // float
    "Ethnicities.Two or More Races",               // float
    "Ethnicities.White Alone",                     // float
    "Ethnicities.White Alone not Hispanic or Latino", // float
    "Income.Median Household Income",              // int
    "Income.Per Capita Income",                    // int
    "Income.Persons Below Poverty Level",          // float
    "Population.2014 Population"                   // int
};


void print_active(struct CorgisStruct* corgis_dataset, int data_size, char *command_buffer) {
    int count = 0;
    for (int i = 0; i < data_size; i++) {
        if (corgis_dataset[i].active == 1) {
            count++;
        }
    }

    printf("%s (%d)\n\n", command_buffer, count);
}


void display_filter(struct CorgisStruct *corgis_dataset, int data_size) {
    printf("%-20s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n\n", 
       "County", "State", "Bac_G", "Hi_G", "Natv", "Asian", "Black", "Hisp", "Pac_Isl", "Mixed", 
       "White", "White_NH", "Med_Inc", "Per_Inc", "Pov_Rate", "Pop_2014");

    for (int i = 0; i < data_size; i++) {
        if (corgis_dataset[i].active == 1) {
            printf("%-20s %-10s %-10.2f %-10.2f %-10.2f %-10.2f %-10.2f %-10.2f %-10.2f %-10.2f %-10.2f %-10.2f %-10d %-10d %-10.2f %-10d\n\n",
                   corgis_dataset[i].county, 
                   corgis_dataset[i].state, 
                   corgis_dataset[i].ed_bac_g, 
                   corgis_dataset[i].ed_hi_g,
                   corgis_dataset[i].eth_native,
                   corgis_dataset[i].eth_asian,
                   corgis_dataset[i].eth_black,
                   corgis_dataset[i].eth_hispanic,
                   corgis_dataset[i].eth_pacific_islander,
                   corgis_dataset[i].eth_mixed,
                   corgis_dataset[i].eth_white,
                   corgis_dataset[i].eth_white_not_hispanic,
                   corgis_dataset[i].inc_med_house,
                   corgis_dataset[i].inc_per_capita,
                   corgis_dataset[i].inc_below_poverty,
                   corgis_dataset[i].pop_2014);
        }
    }
}


void filter_state_filter(struct CorgisStruct *corgis_dataset, int data_size, char *command_buffer) {
    char *command_copy = strdup(command_buffer);

    char *input = command_copy;

    input[strlen(input) - 1] = '\0';

    strsep(&input, ":");

    for (int i = 0; i < data_size; i++) {
        if (corgis_dataset[i].active == 1) {
            if (strcmp(corgis_dataset[i].state, input) != 0) {
                corgis_dataset[i].active = 0;
            }
        }
    }

    print_active(corgis_dataset, data_size, command_copy);

    free(command_copy);
}


void filter_field_filter(struct CorgisStruct *corgis_dataset, int data_size, char *command_buffer) {
    char *command_copy = strdup(command_buffer);

    char *input = command_copy;
    input[strlen(input) - 1] = '\0';

    char **input_list = (char **)(malloc(sizeof(char *) * 4));
    if (input_list == NULL) {
        perror("malloc failed\n");
        free(command_copy);
        return;
    }

    char *token;

    int index = 0;
    while ((token = strsep(&input, ":")) != NULL && index < 4) {
        input_list[index] = strdup(token);
        index++;
    }

    // Check if field exists and if it is numeric
    if (strcmp(input_list[1], fields[0]) == 0 || strcmp(input_list[1], fields[1]) == 0) {
        fprintf(stderr, "Field %s is not numeric\n\n", input_list[1]);
        free(command_copy);
        for (size_t i = 0; i < 4; i++) {
            free(input_list[i]);
        }
        free(input_list);
        return;
    }
    else {
        int i;
        for (i = 2; i < 16; i++) {
            if (strcmp(input_list[1], fields[i]) == 0) {
                break;
            }
        }
        if (i == 16) {
            fprintf(stderr, "Field %s does not exist!\n\n", input_list[1]);
            free(command_copy);
            for (size_t i = 0; i < 4; i++) {
                free(input_list[i]);
            }
            free(input_list);
            return;
        }
    }

    for (int i = 0; i < data_size; i++) {
        if (corgis_dataset[i].active == 1) {
            if (strcmp(input_list[1], "Education.Bachelor's Degree or Higher") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].ed_bac_g < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].ed_bac_g > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Education.High School or Higher") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].ed_hi_g < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].ed_hi_g > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Ethnicities.American Indian and Alaska Native Alone") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].eth_native < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].eth_native > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Ethnicities.Asian Alone") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].eth_asian < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].eth_asian > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Ethnicities.Black Alone") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].eth_black < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].eth_black > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Ethnicities.Hispanic or Latino") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].eth_hispanic < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].eth_hispanic > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].eth_pacific_islander < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].eth_pacific_islander > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Ethnicities.Two or More Races") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].eth_mixed < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].eth_mixed > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Ethnicities.White Alone") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].eth_white < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].eth_white > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Ethnicities.White Alone not Hispanic or Latino") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].eth_white_not_hispanic < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].eth_white_not_hispanic > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Income.Median Household Income") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].inc_med_house < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].inc_med_house > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Income.Per Capita Income") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].inc_per_capita < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].inc_per_capita > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Income.Persons Below Poverty Level") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].inc_below_poverty < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].inc_below_poverty > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else if (strcmp(input_list[1], "Population.2014 Population") == 0) {
                float threshold = atof(input_list[3]);
                for (int i = 0; i < data_size; i++) {
                    if (strcmp(input_list[2], "ge") == 0) {
                        if (corgis_dataset[i].pop_2014 < threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    } else if (strcmp(input_list[2], "le") == 0) {
                        if (corgis_dataset[i].pop_2014 > threshold) {
                            corgis_dataset[i].active = 0;
                        }
                    }
                }
            } else {
                printf("Error: Invalid or unsupported field %s.\n", input_list[1]);
                free(command_copy);
                for (size_t i = 0; i < 4; i++) {
                    free(input_list[i]);
                }
                free(input_list);
                return;
            }
        }
    }

    char *command_copy2 = strdup(command_buffer);
    command_copy2[strlen(command_copy2) - 1] = '\0';
    print_active(corgis_dataset, data_size, command_copy2);
    free(command_copy2);

    free(command_copy);
    for (size_t i = 0; i < 4; i++) {
        free(input_list[i]);
    }
    free(input_list);
}


int population_total_filter(struct CorgisStruct *corgis_dataset, int data_size) {
    int total_population = 0.0f;

    for (int i = 0; i < data_size; i++) {
        if (corgis_dataset[i].active == 1) {
            total_population += corgis_dataset[i].pop_2014;
        }
    }

    printf("2014 population: %d\n\n", total_population);
    return total_population;
}


float population_field_filter(struct CorgisStruct *corgis_dataset, int data_size, char *command_buffer) {
    char *command_copy = strdup(command_buffer);
    char *field = command_copy;
    field[strlen(field) - 1] = '\0';
    strsep(&field, ":");
    
    float total_sub_population = 0.0f;

    // Loop through all entries and calculate the sub-population for the specified field
    for (int i = 0; i < data_size; i++) {
        float sub_population_percentage = 0.0f;
        
        if (corgis_dataset[i].active == 1) {
            // Check which field is specified and get the corresponding value
            if (strcmp(field, "Education.Bachelor's Degree or Higher") == 0) {
                sub_population_percentage = corgis_dataset[i].ed_bac_g / 100;
            } else if (strcmp(field, "Education.High School or Higher") == 0) {
                sub_population_percentage = corgis_dataset[i].ed_hi_g / 100;
            } else if (strcmp(field, "Ethnicities.American Indian and Alaska Native Alone") == 0) {
                sub_population_percentage = corgis_dataset[i].eth_native / 100;
            } else if (strcmp(field, "Ethnicities.Asian Alone") == 0) {
                sub_population_percentage = corgis_dataset[i].eth_asian / 100;
            } else if (strcmp(field, "Ethnicities.Black Alone") == 0) {
                sub_population_percentage = corgis_dataset[i].eth_black / 100;
            } else if (strcmp(field, "Ethnicities.Hispanic or Latino") == 0) {
                sub_population_percentage = corgis_dataset[i].eth_hispanic / 100;
            } else if (strcmp(field, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) {
                sub_population_percentage = corgis_dataset[i].eth_pacific_islander / 100;
            } else if (strcmp(field, "Ethnicities.Two or More Races") == 0) {
                sub_population_percentage = corgis_dataset[i].eth_mixed / 100;
            } else if (strcmp(field, "Ethnicities.White Alone") == 0) {
                sub_population_percentage = corgis_dataset[i].eth_white / 100;
            } else if (strcmp(field, "Ethnicities.White Alone not Hispanic or Latino") == 0) {
                sub_population_percentage = corgis_dataset[i].eth_white_not_hispanic / 100;
            } else if (strcmp(field, "Income.Persons Below Poverty Level") == 0) {
                sub_population_percentage = corgis_dataset[i].inc_below_poverty / 100;
            }

            // Multiply the percentage by the total population for the entry
            float sub_population = corgis_dataset[i].pop_2014 * sub_population_percentage;

            // Add to the total sub-population
            total_sub_population += sub_population;
        }
    }

    // Print the result
    printf("2014 %s population: %.2f\n", field, total_sub_population);

    free(command_copy);

    return total_sub_population;
}


void percent_field_filter(struct CorgisStruct *corgis_dataset, int data_size, char *command_buffer) {
    char *command_copy = strdup(command_buffer);

    // Split the input string by ':'
    char *field = command_copy;
    field[strlen(field) - 1] = '\0';
    strsep(&field, ":");

    float total_population = population_total_filter(corgis_dataset, data_size);
    float sub_population = population_field_filter(corgis_dataset, data_size, command_buffer);

    // Avoid division by zero
    if (total_population > 0) {
        float percentage = (sub_population / total_population) * 100;
        printf("2014 %s percentage: %.2f%%\n\n", field, percentage);
    } else {
        printf("2014 %s percentage: 0%%\n\n", field);
    }

    free(command_copy);
}
