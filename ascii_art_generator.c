#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>

using namespace cv;

// Define a structure to hold ASCII density information
typedef struct {
    int density;
    char ascii_char;
} AsciiMapping;

// Function to load ASCII mappings from a CSV file
int load_ascii_mappings(const char* filename, AsciiMapping** mappings) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open CSV file %s\n", filename);
        return -1;
    }

    // Allocate memory for ASCII mappings
    *mappings = (AsciiMapping*)malloc(128 * sizeof(AsciiMapping));
    int count = 0;

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        int density;
        char ascii_char;
        // Parse the CSV line (format: density,ascii_no,ascii_char)
        if (sscanf(line, "%d,%*d,%c", &density, &ascii_char) == 2) {
            (*mappings)[count].density = density;
            (*mappings)[count].ascii_char = ascii_char;
            count++;
        }
    }

    fclose(file);
    return count;
}

// Function to map grayscale value to ASCII character
char gray_to_ascii(int gray_value, AsciiMapping* mappings, int count) {
    int scaled_density = gray_value * (count - 1) / 255; // Scale gray value to density range
    return mappings[scaled_density].ascii_char;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <image_path> <ascii_csv>\n", argv[0]);
        return 1;
    }

    // Load ASCII mappings from CSV
    AsciiMapping* mappings = NULL;
    int ascii_count = load_ascii_mappings(argv[2], &mappings);
    if (ascii_count <= 0) {
        printf("Error: Failed to load ASCII mappings from CSV file\n");
        return 1;
    }

    // Load image in grayscale
    Mat img = imread(argv[1], IMREAD_GRAYSCALE);
    if (img.empty()) {
        printf("Error: Unable to load image!\n");
        free(mappings);
        return 1;
    }

    // Resize image
    int new_width = 80;
    int new_height = (img.rows * new_width) / img.cols;
    Mat resized_img;
    resize(img, resized_img, Size(new_width, new_height));

    // Open output file
    FILE* output_file = fopen("ascii_art.txt", "w");
    if (!output_file) {
        printf("Error: Unable to create output file!\n");
        free(mappings);
        return 1;
    }

    // Generate ASCII art
    for (int i = 0; i < resized_img.rows; i++) {
        for (int j = 0; j < resized_img.cols; j++) {
            int gray_value = resized_img.at<uchar>(i, j);
            char ascii_char = gray_to_ascii(gray_value, mappings, ascii_count);
            fputc(ascii_char, output_file);  // Write to file
            printf("%c", ascii_char);       // Print to console (optional)
        }
        fputc('\n', output_file);
        printf("\n");
    }

    // Cleanup
    fclose(output_file);
    free(mappings);

    printf("ASCII art saved to ascii_art.txt\n");
    return 0;
}
