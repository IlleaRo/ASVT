#ifndef ASVT_PGM_TOOLS_H
#define ASVT_PGM_TOOLS_H

typedef struct img_params {
    unsigned long header_len;
    unsigned int width;
    unsigned int height;
    unsigned int num_of_color;
} img_params_t;

unsigned char * readImg(const char *filename, img_params_t *params);

int writePGM(const char *filename, unsigned char *image, img_params_t *params);

int get_noise(unsigned char *img, unsigned long size, unsigned int header_size);

int medianFilter(const unsigned char *input_p, unsigned char *output_p, img_params_t *params);

#endif //ASVT_PGM_TOOLS_H
