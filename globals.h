#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <grass/raster.h>

/*
 * openfiles.c
 * hue, saturation, intensity,
 * red, green, blue,
 * int fd_input[3], int fd_output[3],
 * DCELL * rowbuf[3]
 */
void openfiles(char *, char *, char *, char *, char *, char *, int[3], int[3],
        DCELL *[3]);

/*
 * his2rgb.c
 * rowbuffer
 * columns
 * maximum number of colors
 */
void his2rgb(DCELL *[3], unsigned int, unsigned int);

/*
 * closefiles.c
 * red, green, blue
 * output file descriptors
 * output rowbuffers
 */
int closefiles(char *, char *, char *, int[3], DCELL *[3]);

#endif /* __GLOBALS_H__ */
