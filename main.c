#include "pgm_tools/pgm_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define NUM_OF_STEPS 3
#define MAX_MSG_SIZE 50

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Use ./filter <file>");
        exit(EXIT_FAILURE);
    }

    img_params_t params;
    srand(time(NULL));

    char msg[MAX_MSG_SIZE];
    long decision;
    char *endPtr;

    unsigned char *inputImg = readImg(argv[1], &params);

    if (!inputImg) {
        fprintf(stderr, "Can't read image or memory allocate error!\n");
        exit(EXIT_FAILURE);
    }

    unsigned char *outputImg = (unsigned char *)malloc((params.header_len + params.height * params.width) * sizeof(unsigned char));

    memcpy(outputImg, inputImg, params.header_len + params.height * params.width);

    if (!outputImg) {
        free(inputImg);
        fprintf(stderr, "Memory allocate error!");
        exit(EXIT_FAILURE);
    }

    printf("\nSelect action:\n1) Add pulse noise\n2) Use the median filter\n3) Save image\n4) Exit");

    while (1) {
        printf("\n---> ");
        fgets(msg, MAX_MSG_SIZE, stdin);
        decision = strtol(msg, &endPtr, 10);

        if (decision == 0 || decision == LONG_MAX || decision == LONG_MIN) {
            printf("\n---> ");
            continue;
        }

        switch (decision) {
            case 1: {
                if (get_noise(inputImg, params.height * params.width + params.header_len,
                          params.header_len)) {
                    fprintf(stderr, "Unexpected error!\n");
                    free(inputImg);
                    free(outputImg);
                    exit(EXIT_FAILURE);
                }
                memcpy(&outputImg[params.header_len ], &inputImg[params.header_len],
                       params.height * params.width);
                break;
            }
            case 2: {

                unsigned char *tmpBuf = (unsigned char *)malloc((params.header_len + params.height * params.width) * sizeof(unsigned char));

                if (!tmpBuf) {
                    fprintf(stderr, "Can't read image or memory allocate error!\n");
                    exit(EXIT_FAILURE);
                }

                memcpy(tmpBuf, inputImg, params.header_len + params.height * params.width);

                for (int i = 0; i < NUM_OF_STEPS; ++i) {
                    if (medianFilter(tmpBuf, outputImg, &params)) {
                        fprintf(stderr, "Unexpected error!\n");
                        free(inputImg);
                        free(outputImg);
                        exit(EXIT_FAILURE);
                    }
                    memcpy(&tmpBuf[params.header_len], &outputImg[params.header_len],
                           params.height * params.width);
                }
                free(tmpBuf);
                break;
            }
            case 3: {
                if (writePGM("output.pgm", outputImg, &params)) {
                    fprintf(stderr, "Can't write image!\n");
                    free(inputImg);
                    free(outputImg);
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 4: {
                free(inputImg);
                free(outputImg);
                exit(EXIT_SUCCESS);
            }
            default: {
                printf("\n---> ");
                continue;
            }
        }
    }
}