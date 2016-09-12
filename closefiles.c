#include <stdlib.h>
#include <grass/gis.h>
#include <grass/raster.h>
#include "globals.h"

/* This routine closes up the cell maps, frees up the row buffers and
   use a less than perfect way of setting the color maps for the output
   to grey scale.  */

int closefiles(char *red, char *green, char *blue,
               int fd_output[3],
               DCELL * rowbuf[3])

{
    unsigned int color;
    struct Colors colors;
    struct FPRange range;
    struct History history;
    DCELL min, max;     // set to [0, max_colors] instead of [min, max] ?
    const char *mapset;

    for (color = 0; color < 3; color++)
    {
        Rast_close(fd_output[color]);
        G_free(rowbuf[color]);
    }

    mapset = G_mapset();

    /* write colors */
    Rast_read_fp_range(red, mapset, &range);
    Rast_get_fp_range_min_max(&range, &min, &max);
    Rast_make_grey_scale_colors(&colors, min, max);
    Rast_write_colors(red, mapset, &colors);

    Rast_read_fp_range(green, mapset, &range);
    Rast_get_fp_range_min_max(&range, &min, &max);
    Rast_make_grey_scale_colors(&colors, min, max);
    Rast_write_colors(green, mapset, &colors);

    Rast_read_fp_range(blue, mapset, &range);
    Rast_get_fp_range_min_max(&range, &min, &max);
    Rast_make_grey_scale_colors(&colors, min, max);
    Rast_write_colors(blue, mapset, &colors);

    /* write metadata */
    Rast_short_history(red, "raster", &history);
    Rast_command_history(&history);
    Rast_write_history(red, &history);
    Rast_put_cell_title(red, "Image red");

    Rast_short_history(green, "raster", &history);
    Rast_command_history(&history);
    Rast_write_history(green, &history);
    Rast_put_cell_title(green, "Image green");

    Rast_short_history(blue, "raster", &history);
    Rast_command_history(&history);
    Rast_write_history(blue, &history);
    Rast_put_cell_title(blue, "Image blue");

    return 0;
}
