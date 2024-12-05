#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <string.h>
#include "corgis_struct.h"
#include "helpers.h"
#include "operations.h"


int filter_interpreter(char *command_buffer) {
    char *command = strdup(command_buffer);
    int command_num = -1;

    int len = strlen(command);
    if (len > 0 && command[len - 1] == '\n') {
        command[len - 1] = '\0';
    }
    
    if (strcmp(command, "display") == 0) {
        command_num = 1;
    }
    else if (strncmp(command, "filter-state:", 13) == 0) {
        command_num = 2;
    }
    else if (strncmp(command, "filter:", 7) == 0) {
        command_num = 3;
    }
    else if (strcmp(command, "population-total") == 0) {
        command_num = 4;
    }
    else if (strncmp(command, "population:", 11) == 0) {
        command_num = 5;
    }
    else if (strncmp(command, "percent:", 8) == 0) {
        command_num = 6;
    }
    else {
        command_num = -1;
    }

    free(command);
    return command_num;
}


int get_data_size(FILE *demo_data) {
    int data_size = 0;

    for (int c = getc(demo_data); c != EOF; c = getc(demo_data)) {
        if (c == '\n') {
            data_size++;
        }
    }

    rewind(demo_data);

    return data_size;
}


struct CorgisStruct *corgis_loader(FILE *demo_data, int *data_size) {
    *data_size = get_data_size(demo_data); 
    if (data_size == 0) {
        return NULL;
    }

    // Remove column header
    *data_size = *data_size - 1;

    struct CorgisStruct *corgis_dataset = (struct CorgisStruct*)(malloc(*data_size * sizeof(struct CorgisStruct)));
    if (corgis_dataset == NULL) {
        perror("malloc failed\n");
        return NULL;
    }

    char *line_buf = NULL;
    size_t len = 0;
    ssize_t read;
    int index = 0;

    // Skip the first entry of the stream
    read = getline(&line_buf, &len, demo_data);

    while ((read = getline(&line_buf, &len, demo_data)) != -1) {
        size_t count;

        remove_quotes(line_buf);
        char **result = split_string(line_buf, ",", &count);
        
        // Check if entry is malformed
        if (count != 52) {
            fprintf(stderr, "Entry %d is malformed!\n", index - 1);
            *data_size = *data_size - 1;
            index++;
            continue;
        }

        strcpy(corgis_dataset[index].county, result[0]);
        strcpy(corgis_dataset[index].state, result[1]);
        corgis_dataset[index].ed_bac_g = atof(result[5]);
        corgis_dataset[index].ed_hi_g = atof(result[6]);
        corgis_dataset[index].eth_native = atof(result[11]);
        corgis_dataset[index].eth_asian = atof(result[12]);
        corgis_dataset[index].eth_black = atof(result[13]);
        corgis_dataset[index].eth_hispanic = atof(result[14]);
        corgis_dataset[index].eth_pacific_islander = atof(result[15]);
        corgis_dataset[index].eth_mixed = atof(result[16]);
        corgis_dataset[index].eth_white = atof(result[17]);
        corgis_dataset[index].eth_white_not_hispanic = atof(result[18]);
        corgis_dataset[index].inc_med_house = atoi(result[25]);
        corgis_dataset[index].inc_per_capita = atoi(result[26]);
        corgis_dataset[index].inc_below_poverty = atof(result[27]);
        corgis_dataset[index].pop_2014 = atoi(result[38]);
        corgis_dataset[index].active = 1;

        free_string_list(result, count);
        index++;
    }

    free(line_buf);
    return corgis_dataset;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid argument count!\n");
        return 1;
    }

    FILE *demo_data = fopen(argv[1], "r");
    if (demo_data == NULL) {
        perror("File not found\n");
        return 1;
    }

    FILE *operations = fopen(argv[2], "r");
    if (operations == NULL) {
        perror("File not found \n");
        return 1;
    }

    int data_size = 0;
    struct CorgisStruct *corgis_dataset = corgis_loader(demo_data, &data_size);
    if (corgis_dataset == NULL) {
        fprintf(stderr, "NULL Dataset!\n");
        fclose(demo_data);
        fclose(operations);
        return 1;
    }
    else {
        printf("%d entries loaded successfully!\n", data_size);
    }

    char *command_buffer = NULL;
    size_t len = 0;
    ssize_t read;
    int filter = -1;
    while ((read = getline(&command_buffer, &len, operations)) != -1) {
        filter = filter_interpreter(command_buffer);

        switch (filter) {
            case 1:
                display_filter(corgis_dataset, data_size);
                break;

            case 2:
                filter_state_filter(corgis_dataset, data_size, command_buffer);
                break;

            case 3:
                filter_field_filter(corgis_dataset, data_size, command_buffer);
                break;

            case 4:
                population_total_filter(corgis_dataset, data_size);
                break;

            case 5:
                population_field_filter(corgis_dataset, data_size, command_buffer);
                break;

            case 6:
                percent_field_filter(corgis_dataset, data_size, command_buffer);
                break;

            default:
                fprintf(stderr, "Invalid filter specified!\n\n");
        }
    }

    printf("\n");
    free(command_buffer);
    free(corgis_dataset);
    fclose(demo_data);
    fclose(operations);
    return 0;
}
