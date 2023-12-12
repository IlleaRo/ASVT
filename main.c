#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quick_sort/quick_sort.h"



typedef struct img_params {
    unsigned long header_len;
    unsigned int width;
    unsigned int height;
    unsigned int num_of_color;
} img_params_t;

unsigned char * readImg(const char *filename, img_params_t *params) {
    if (!filename || !params) {
        return NULL;
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Can't open image %s!", "input.pgm");
        exit(EXIT_FAILURE);
    }

    char read_string[4][256];
    params->header_len = 0;
    for (int i = 0; i < 4; ++i) {
        if (!fgets(read_string[i], 255, file)) {
            fprintf(stderr, "Incorrect format!");
            exit(EXIT_FAILURE);
        }
        params->header_len += strlen(read_string[i]);
    }

    if (sscanf(read_string[2], "%d %d", &params->width, &params->height) < 2) {
        fprintf(stderr, "Incorrect header!");
        exit(EXIT_FAILURE);
    }
    if (sscanf(read_string[3], "%d", &params->num_of_color) < 1) {
        fprintf(stderr, "Incorrect header!");
        exit(EXIT_FAILURE);
    }


    unsigned char *res_img = (unsigned char *) malloc((params->header_len + params->height * params->width) * sizeof(unsigned char));
    if (!res_img) {
        return NULL;
    }

    unsigned char *cur_pos = res_img;
    for (int i = 0; i < 4; ++i) {
        cur_pos = memccpy(cur_pos, read_string[i], '\n', params->header_len);
    }


    if (fread(cur_pos, sizeof(unsigned char), params->height * params->width, file) != params->width * params->height) {
        fprintf(stderr, "Broken data!");
        exit(EXIT_FAILURE);
    }

    fclose(file);

    return res_img;
}

// Функция для записи PGM-изображения в файл
void writePGM(const char *filename, unsigned char *image, img_params_t *params) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    fwrite(image, sizeof(unsigned char), (params->header_len * params->width * params->height), file); // Запись данных изображения
    fclose(file);
}

// Функция для применения медианного фильтра к изображению
void medianFilter(const unsigned char *input_p, unsigned char *output_p, img_params_t *params) {
    unsigned char *output = &output_p[params->header_len];
    unsigned char *input = &input_p[params->header_len];

    unsigned char window[13];
    for (int i = 0; i < params->height; i++) {
        for (int j = 0; j < params->width; j++) {

            //central
            window[12] = input[i * params->width + j];

            // left part
            window[0] = (j - 3 >= 0) ? input[i * params->width + j - 3] : input[i * params->width + j];
            window[1] = (j - 2 >= 0) ? input[i * params->width + j - 2] : input[i * params->width + j];
            window[2] = (j - 1 >= 0) ? input[i * params->width + j - 1] : input[i * params->width + j];

            // right part
            window[3] = (j + 1 < params->width) ? input[i * params->width + j + 1] : input[i * params->width + j];
            window[4] = (j + 2 < params->width) ? input[i * params->width + j + 2] : input[i * params->width + j];
            window[5] = (j + 3 < params->width) ? input[i * params->width + j + 3] : input[i * params->width + j];

            // upper part
            window[6] = (i - 3 >= 4) ? input[(i - 3) * params->width + j] : input[i * params->width + j];
            window[7] = (i - 2 >= 4) ? input[(i - 2) * params->width + j] : input[i * params->width + j];
            window[8] = (i - 1 >= 4) ? input[(i - 1) * params->width + j] : input[i * params->width + j];

            // bottom part
            window[9]  = (i + 1 < params->height) ? input[(i + 1) * params->width + j] : input[i * params->width + j];
            window[10] = (i + 2 < params->height) ? input[(i + 2) * params->width + j] : input[i * params->width + j];
            window[11] = (i + 3 < params->height) ? input[(i + 3) * params->width + j] : input[i * params->width + j];

            sort(window, 13);

            output[i * params->width + j] = window[6];
        }
    }
}

int main() {
    unsigned int height, width;
    img_params_t params;

    // Read source image


    unsigned char *inputImg = readImg("input.pgm", &params);
    unsigned char *outputImg = (unsigned char *)malloc((params.header_len + params.height * params.width) * sizeof(unsigned char));
    memcpy(outputImg, inputImg, params.header_len + params.height * params.width);

    if (!inputImg || !outputImg) {
        fprintf(stderr, "Memory allocate error!");
        exit(EXIT_FAILURE);
    }

    // Применение медианного фильтра
    medianFilter(inputImg, outputImg, &params);

    // Запись выходного изображения
    writePGM("output.pgm", outputImg, &params);

    // Освобождение памяти
    free(inputImg);
    free(outputImg);

    return 0;
}