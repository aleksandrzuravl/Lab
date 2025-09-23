#ifndef BMP_READER_H
#define BMP_READER_H

#include <cstdint> // For precise bit control
#include <iostream>
#include <memory>

#pragma pack(push, 1) // Disable compiler alignment of structures

// Structure for a BMP file header
class BMP_Header {
public:
    uint8_t ID[2]; // File identifier ('BM')
    uint32_t file_size; // BMP file size
    uint8_t unused[4]; // Reserved data
    uint32_t pixel_offset; // Offset to the start of pixel data

    BMP_Header() = default;
};

// Structure for DIB header
class DIB_Header {
public:
    uint32_t header_size; // DIB header size
    uint32_t width; // Image width in pixels
    uint32_t height; // Image height in pixels
    uint16_t color_planes; // Number of color planes
    uint16_t bits_per_pixel; // Number of bits per pixel
    uint32_t compression; // Compression type
    uint32_t data_size; // Size of pixel data
    uint32_t print_resolution_x; // Resolution along the X axis (in pixels per meter)
    uint32_t print_resolution_y; // Y-axis resolution (in pixels per meter)
    uint32_t colors_count; // Number of colors in the palette
    uint32_t important_colors; // Number of important colors

    DIB_Header() = default;
};

// Structure for RGB pixel data (24-bit color)
class RGB {
public:
    uint8_t blue;  // 8 bits for Blue channel
    uint8_t green; // 8 bits for Green channel
    uint8_t red;   // 8 bits for Red channel

    // Default constructor
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : blue(b), green(g), red(r) {}
};

// Structure for the BMP file
class BMP_File {
public:
    BMP_Header bmp_header; // Header BMP
    DIB_Header dib_header; // Reader BIB
    RGB* file_data; // Pixel data

    BMP_File() : file_data(nullptr) {}
    
    ~BMP_File() {
        delete[] file_data;
    }

    // Methods Load and Save BMP File
    virtual bool Load_BMP_File(const char* file_name);
    virtual void Save_BMP_File(const char* output_filename);

    // Methods flipped BMP File
    std::unique_ptr<BMP_File> flip_BMP_90_contra_clockwise();
    std::unique_ptr<BMP_File> flip_BMP_90_clockwise();
};

#pragma pack(pop)  // Shutdown <pragma pack(push, 1)>

#endif
