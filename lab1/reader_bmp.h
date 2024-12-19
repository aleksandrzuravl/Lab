#ifndef BMP_READER_H
#define BMP_READER_H

#include <cstdint> 
#include <iostream>
#include <memory>

#pragma pack(push, 1) 

struct BMP_Header{
    uint8_t ID[2]; // file identifier ('BM')
    uint32_t file_size; // BMP file size
    uint8_t unused[4]; // reserved data
    uint32_t pixel_offset; // offset to the start of pixel data

    BMP_Header() = default;
};


struct DIB_Header{
    uint32_t header_size; // DIB header size
    uint32_t width; // in pixels
    uint32_t height; 
    uint16_t color_planes;
    uint16_t bits_per_pixel; // bits per pixel
    uint32_t compression; 
    uint32_t data_size; // size of pixel data
    uint32_t print_resolution_x; // resolution along the X axis (in pixels per meter)
    uint32_t print_resolution_y; // Y axis resolution (in pixels per meter)
    uint32_t colors_count; // number of colors
    uint32_t important_colors; // number of important colors

    DIB_Header() = default;
};

struct RGB {
    uint8_t blue;  // 8 bits for Blue channel
    uint8_t green; // 8 bits for Green channel
    uint8_t red;   // 8 bits for Red channel

    // default constructor
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : blue(b), green(g), red(r) {}
};

class BMP_File {
public:
    BMP_Header bmp_header; // header BMP
    DIB_Header dib_header; // reader DIB
    RGB* file_data; // pixel data

    BMP_File() : file_data(nullptr) {}
    
    ~BMP_File() {
        delete[] file_data;
    }

    // load and save BMP File
    virtual bool Load_image(const char* file_name);
    virtual void Save_image(const char* output_filename);

    // rotate BMP File
    std::unique_ptr<BMP_File> rotate_contra_clockwise();
    std::unique_ptr<BMP_File> rotate_clockwise();
};

#pragma pack(pop)  

#endif
