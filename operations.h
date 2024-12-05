#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "./corgis_struct.h"

void display_filter(struct CorgisStruct *corgis_dataset, int data_size);
void filter_state_filter(struct CorgisStruct *corgis_dataset, int data_size, char *command_buffer);
void filter_field_filter(struct CorgisStruct *corgis_dataset, int data_size, char *command_buffer);
int population_total_filter(struct CorgisStruct *corgis_dataset, int data_size);
float population_field_filter(struct CorgisStruct *corgis_dataset, int data_size, char *command_buffer);
void percent_field_filter(struct CorgisStruct *corgis_dataset, int data_size, char *command_buffer);

#endif  // OPERATIONS_H_
