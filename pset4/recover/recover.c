#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Chack for correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    // Open file
    FILE *file = fopen(argv[1], "r");

    // Check if file was opened
    if (!file)
    {
        printf("Could not open the file\n");
        return 1;
    }

    // Innitialize number of JPEGS and declare JPEG's pointer
    int nmbr_of_img = 0;
    FILE *image_file;

    // Read the file, 512 bytes at a time
    BYTE *buffer = malloc(512 * sizeof(BYTE));
    bool writing = false;
    while (fread(buffer, sizeof(BYTE), 512, file))
    {
        // Look for JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If not first JPEG, close current file and update number of images already found
            if (writing)
            {
                fclose(image_file);
                nmbr_of_img++;
            }
            
            // Create name for the JPEG found
            char filename[8];
            sprintf(filename, "%03i.jpg", nmbr_of_img);

            // Open new file
            image_file = fopen(filename, "w");
            
            // Notify program first JPEG was already found so it writes read content
            writing = true;
        }

        // If a JPEG was already found, keep writing into it
        if (writing)
        {
            fwrite(buffer, sizeof(BYTE), 512, image_file);
        }
    }
    
    fclose(image_file);
    fclose(file);
    free(buffer);
}