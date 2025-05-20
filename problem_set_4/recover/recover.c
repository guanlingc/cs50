#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }
    // Open the memory card
    char *infile = argv[1];
    FILE *input = fopen(infile, "r");
    // While there's still data left to read from the memory card
    if (input == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // Create JPEGs from the data
    // assign block to store image
    unsigned char buffer[512];
    FILE *output = NULL;
    // filename for output
    char filename[8];
    int jpeg_count = 0;
    // read block by block
    while (fread(buffer, 1, 512, input) == 512)
     {
        // Check if the current block starts a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            buffer[3] >= 0xe0 && buffer[3] <= 0xef)
        {
            // If we've already found a JPEG, close the previous one
            if (output != NULL)
            {
                fclose(output);
            }

            // Create a new JPEG filename
            sprintf(filename, "%03d.jpg", jpeg_count++);

            // Open a new output file for writing
            output = fopen(filename, "w");
            if (output == NULL)
            {
                fclose(input);
                fprintf(stderr, "Could not open %s for writing.\n", filename);
                return 1;
            }

            // Write the current block to the new JPEG file
            fwrite(buffer, 1, 512, output);
        }
        // If we've already found a JPEG, continue writing to it
        else if (output != NULL)
        {
            fwrite(buffer, 1, 512, output);
        }
    }

    // Close the last opened JPEG file, if any
    if (output != NULL)
    {
        fclose(output);
    }

    // Close the input file
    fclose(input);

    return 0;
}
