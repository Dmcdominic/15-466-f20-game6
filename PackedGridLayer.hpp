
#pragma once
struct PackedGridLayer {
    unsigned int level_id;
    unsigned int layer_number; //bg=0, fg=1, sky=2
    unsigned int difficulty; //easy=0, med=1, hard=2
    unsigned int data_start; 
    unsigned int width; 
    unsigned int height; 
};