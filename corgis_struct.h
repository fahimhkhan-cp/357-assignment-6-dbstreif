#ifndef CORGIS_STRUCT_H_
#define CORGIS_STRUCT_H_

struct CorgisStruct {
    char county[1500];
    char state[3];
    float ed_bac_g;
    float ed_hi_g;
    float eth_native;
    float eth_asian;
    float eth_black;
    float eth_hispanic;
    float eth_pacific_islander;
    float eth_mixed;
    float eth_white;
    float eth_white_not_hispanic;
    int inc_med_house;
    int inc_per_capita;
    float inc_below_poverty;
    int pop_2014;
    int active;
};

#endif  // CORGIS_STRUCT_H_
