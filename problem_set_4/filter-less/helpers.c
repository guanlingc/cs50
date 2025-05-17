#include "helpers.h"
#include <math.h>

// Convert image to grayscale
// row height
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop thru all the rows (for each row)
    for (int i = 0; i < height; i++)
    {
        // loop thru each column (i.e. each cell)
        for (int j = 0; j < width; j++)
        {
            // find the average of the cell
            int avg =
                round(((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0));
            // set avg to same data type
            BYTE gray = avg;
            // set all 3 channels at the avg value
            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = gray;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // loop thru all the rows (for each row)
    for (int i = 0; i < height; i++)
    {
        // loop thru each column (i.e. each cell)
        for (int j = 0; j < width; j++)
        {
            // define the original image's rgb
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;
            // calculate sepia value
            int sepia_red = round(.393 * red + .769 * green + .189 * blue);
            if (sepia_red > 255)
            {
                sepia_red = 255;
            }
            int sepia_green = round(.349 * red + .686 * green + .168 * blue);
            if (sepia_green > 255)
            {
                sepia_green = 255;
            }
            int sepia_blue = round(.272 * red + .534 * green + .131 * blue);
            if (sepia_blue > 255)
            {
                sepia_blue = 255;
            }
            // reassign sepia back to pixel
            BYTE new_red = sepia_red;
            BYTE new_green = sepia_green;
            BYTE new_blue = sepia_blue;
            image[i][j].rgbtRed = new_red;
            image[i][j].rgbtGreen = new_green;
            image[i][j].rgbtBlue = new_blue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // loop thru each column (i.e. each cell)
        int offset = 1;
        for (int j = 0; j < width / 2; j++)
        {
            // store current cell info in tmp
            int length_of_row = width;
            RGBTRIPLE temp = image[i][j];
            // assign destination cell values to current cell
            image[i][j] = image[i][length_of_row - offset];
            // assign tmp (current cell) to destination cell
            image[i][length_of_row - offset] = temp;
            offset += 1;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // make a copy
    RGBTRIPLE copy[height][width];
    // loop thru all the rows (for each row)
    for (int i = 0; i < height; i++)
    {
        // loop thru each column (i.e. each cell)
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    // get values to calculate average
    // loop thru all the rows (for each row)
    for (int i = 0; i < height; i++)
    {
        // loop thru each column (i.e. each cell)
        for (int j = 0; j < width; j++)
        {
            // track the color channels
            float blue_sum = 0;
            float green_sum = 0;
            float red_sum = 0;
            float neighbor_count = 0.0;
            // look at each values in the neighbors
            // look at the previous row to next row
            for (int row = -1; row <= 1; row++)
            {
                // look at previous column to next column
                for (int col = -1; col <= 1; col++)
                {
                    // track current cell
                    int current_row = i + row;
                    int current_col = j + col;

                    // Check if the neighbor is within the image boundaries
                    if (current_row >= 0 && current_row < height && current_col >= 0 &&
                        current_col < width)
                    {
                        blue_sum += copy[current_row][current_col].rgbtBlue;
                        green_sum += copy[current_row][current_col].rgbtGreen;
                        red_sum += copy[current_row][current_col].rgbtRed;
                        neighbor_count++;
                    }
                }
            }
            // loop ends here and get the average
            BYTE blur_blue = round(blue_sum / neighbor_count);
            BYTE blur_green = round(green_sum / neighbor_count);
            BYTE blur_red = round(red_sum / neighbor_count);
            // assign blur values into the cell
            image[i][j].rgbtBlue = blur_blue;
            image[i][j].rgbtGreen = blur_green;
            image[i][j].rgbtRed = blur_red;
        }
    }
    return;
}
