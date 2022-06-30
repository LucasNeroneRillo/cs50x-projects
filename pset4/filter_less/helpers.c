#include "helpers.h"
#include <math.h>

int constrain(int x, int min, int max);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Create an empty 2D array to store converted image
    RGBTRIPLE image_copy[height][width];

    // Iterate through all pixels from image
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate total ammount of colar in pixel
            int total_color = 0;
            total_color += image[i][j].rgbtBlue;
            total_color += image[i][j].rgbtGreen;
            total_color += image[i][j].rgbtRed;

            // Calculate filtered pixel and apply it to image's copy
            int gray = round(total_color / 3.0);
            image_copy[i][j].rgbtBlue = gray;
            image_copy[i][j].rgbtGreen = gray;
            image_copy[i][j].rgbtRed = gray;
        }
    }

    // Update original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Create an empty 2D array to store converted image
    RGBTRIPLE image_copy[height][width];

    // Iterate through all pixels from image
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate new RGB according to sepia algorithm
            int old_r = image[i][j].rgbtRed;
            int old_g = image[i][j].rgbtGreen;
            int old_b = image[i][j].rgbtBlue;
            int new_r = round(.393 * old_r + .769 * old_g + .189 * old_b);
            int new_g = round(.349 * old_r + .686 * old_g + .168 * old_b);
            int new_b = round(.272 * old_r + .534 * old_g + .131 * old_b);
            
            // Apply filtered pixel (limiting RBG to range [0, 255])
            image_copy[i][j].rgbtBlue = constrain(new_b, 0, 255);
            image_copy[i][j].rgbtGreen = constrain(new_g, 0, 255);
            image_copy[i][j].rgbtRed = constrain(new_r, 0, 255);
        }
    }

    // Update original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Sawp pixels
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create an empty 2D array to store converted image
    RGBTRIPLE image_copy[height][width];

    // Iterate through all pixels from image
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            // Innitialize number of pixel's neighbours and total color in them
            int total_pixels = 0;
            int total_blue = 0, total_green = 0, total_red = 0;

            // Iterate through neighbour pixels
            for (int m = i - 1; m <= i + 1; m++)
            {
                for (int n = j - 1; n <= j + 1; n++)
                {
                    // Check if neighbour is within image's limits
                    if (m >= 0 && m < height && n >= 0 && n < width)
                    {
                        // Update variables
                        total_blue += image[m][n].rgbtBlue;
                        total_green += image[m][n].rgbtGreen;
                        total_red += image[m][n].rgbtRed;

                        total_pixels++;
                    }
                }
            }

            // Filter pixel by changing their values to neighbours' average
            int avg_blue = round(total_blue / (float) total_pixels);
            int avg_green = round(total_green / (float) total_pixels);
            int avg_red = round(total_red / (float) total_pixels);
            image_copy[i][j].rgbtBlue = avg_blue;
            image_copy[i][j].rgbtGreen = avg_green;
            image_copy[i][j].rgbtRed = avg_red;
        }
    }

    // Update original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }

    return;
}

// Function to constrain x to a given range
int constrain(int x, int min, int max)
{
    if (x < min)
    {
        return min;
    }
    else if (x > max)
    {
        return max;
    }
    else
    {
        return x;
    }
}