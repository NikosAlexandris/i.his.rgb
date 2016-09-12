
/******************************************************************************
NAME:       HIS to RGB

PURPOSE:    To proces hue, intensity, saturation bands to red, green, blue.

ALGORITHM:  Get hue, intensity, saturation from input buffer
            Create the RGB bands
            Write to output buffer

ASSUMPTION: The input images are read to the input buffer.

NOTE:       For GRASS one row from each cell map is passed in and each cell in
            each band is processed and written out.   CWU GIS Lab: DBS 8/90

******************************************************************************/

#include <grass/gis.h>
#include "globals.h"

void his2rgb(DCELL *rowbuffer[3], unsigned int columns, unsigned int max_colors)
{
    long column;        // column indicator
    double red;         // red output image
    double green;       // green output image
    double blue;        // blue output image
    double scaled_red;  // ranging in [0,1], used to compute hue
    double scaled_green;// likewise 
    double scaled_blue; // likewise
    double m1;          // value used to determine RGB
    double m2;          // value used to determine RGB
    double scaled_intensity;    // input image ranging in [0,1]
    double scaled_saturation;   // input image ranging in [0,1]
    double hue;         // input image ranging in [0, 360]
    double savehue;     // save the hue for future processing

    /* loop over columns and appropriately set NULLs */
    for (column = 0; column < columns; column++)
    {
        if (Rast_is_d_null_value(&rowbuffer[0][column]) ||
                Rast_is_d_null_value(&rowbuffer[1][column]) ||
                Rast_is_d_null_value(&rowbuffer[2][column]))
        {
            Rast_set_d_null_value(&rowbuffer[0][column], 1);
            Rast_set_d_null_value(&rowbuffer[1][column], 1);
            Rast_set_d_null_value(&rowbuffer[2][column], 1);
            continue;
        }

        /* initialize zero red, green, blue */
        red = green = blue = 0.0;

        /* input intensity (from i.rgb.his) ranges in [0,1] */
        scaled_intensity = rowbuffer[1][column];

        /* input saturation (from i.rgb.his) ranges in [0,1] */
        scaled_saturation = rowbuffer[2][column];

        m2 = 0.0;

        if (scaled_intensity <= 0.50)
            m2 = scaled_intensity * (1.0 + scaled_saturation);

        else if (scaled_intensity > 0.50)
            m2 = scaled_intensity + scaled_saturation - (scaled_intensity * scaled_saturation);

        m1 = 2.0 * scaled_intensity - m2;

        /* input hue (from i.rgb.his) ranges in [0, 360] */
        hue = rowbuffer[0][column];

        if (scaled_saturation == 0.0) {
            if (hue == -1.0) {
                red = scaled_intensity;
                green = scaled_intensity;
                blue = scaled_intensity;
            }
        }
        else {
            /* calculate the red band */
            savehue = hue + 120.0;
            if (savehue > 360.0)
                savehue -= 360.0;
            if (savehue < 0.0)
                savehue += 360.0;
            if (savehue < 60.0)
                red = m1 + (m2 - m1) * savehue / 60.0;
            else if (savehue < 180.0)
                red = m2;
            else if (savehue < 240.0)
                red = m1 + (m2 - m1) * (240.0 - savehue) / 60.0;
            else
                red = m1;

            /* calculate the green band */
            savehue = hue;
            if (savehue > 360.0)
                savehue -= 360.0;
            if (savehue < 0.0)
                savehue += 360.0;
            if (savehue < 60.0)
                green = m1 + (m2 - m1) * savehue / 60.0;
            else if (savehue < 180.0)
                green = m2;
            else if (savehue < 240.0)
                green = m1 + (m2 - m1) * (240.0 - savehue) / 60.0;
            else
                green = m1;

            /* calculate the blue band */
            savehue = hue - 120.0;
            if (savehue > 360.0)
                savehue -= 360.0;
            if (savehue < 0.0)
                savehue += 360.0;
            if (savehue < 60.0)
                blue = m1 + (m2 - m1) * savehue / 60.0;
            else if (savehue < 180.0)
                blue = m2;
            else if (savehue < 240.0)
                blue = m1 + (m2 - m1) * (240.0 - savehue) / 60.0;
            else
                blue = m1;
        }

        scaled_red = red * max_colors;
        scaled_green = green * max_colors;
        scaled_blue = blue * max_colors;

        if (scaled_red > max_colors)
            red = max_colors;

        else
            red = scaled_red;

        if (scaled_green > max_colors)
            green = max_colors;

        else
            green = scaled_green;

        if (scaled_blue > max_colors)
            blue = max_colors;

        else
            blue = scaled_blue;

        if (red < 0.0)
            red = 0.0;

        if (green < 0.0)
            green = 0.0;

        if (blue < 0.0)
            blue = 0.0;

        G_debug(2, "Red, Green, Blue [scaled up to]: %f, %f, %f, [%.d]",
                red, green, blue, max_colors);

        /* write value into corresponding row, col */
        rowbuffer[0][column] = red;
        rowbuffer[1][column] = green;
        rowbuffer[2][column] = blue;
    }
}
