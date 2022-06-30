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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create an empty 2D array to store converted image
    RGBTRIPLE image_copy[height][width];

    // Iterate through all pixels from image
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            // Innitialize "Sober operator" variables
            int gx_b = 0, gx_g = 0, gx_r = 0;
            int gy_b = 0, gy_g = 0, gy_r = 0;

            // Iterate through neighbours to calculate Gx and Gy
            for (int m = i - 1; m <= i + 1; m++)
            {
                for (int n = j - 1; n <= j + 1; n++)
                {
                    // Check if  neighbour is within image's limits
                    if (m >= 0 && m < height && n >= 0 && n < width)
                    {
                        // Calculate pixel's weight for Gx and Gy
                        int gx_weight = n - j;
                        int gy_weight = m - i;
                        if (m == i)
                        {
                            gx_weight *= 2;
                        }
                        if (n == j)
                        {
                            gy_weight *= 2;
                        }

                        // Update Gx and Gy
                        gx_b += gx_weight * image[m][n].rgbtBlue;
                        gx_g += gx_weight * image[m][n].rgbtGreen;
                        gx_r += gx_weight * image[m][n].rgbtRed;
                        gy_b += gy_weight * image[m][n].rgbtBlue;
                        gy_g += gy_weight * image[m][n].rgbtGreen;
                        gy_r += gy_weight * image[m][n].rgbtRed;
                    }
                }
            }

            // Calculate filtered pixels
            int blue = pow(gx_b, 2) + pow(gy_b, 2);
            int green = pow(gx_g, 2) + pow(gy_g, 2);
            int red = pow(gx_r, 2) + pow(gy_r, 2);
            blue = round(sqrt((float) blue));
            green = round(sqrt((float) green));
            red = round(sqrt((float) red));

            // Apply filtered pixel (limiting RBG to range [0, 255])
            image_copy[i][j].rgbtBlue = constrain(blue, 0, 255);
            image_copy[i][j].rgbtGreen = constrain(green, 0, 255);
            image_copy[i][j].rgbtRed = constrain(red, 0, 255);
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