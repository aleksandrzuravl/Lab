#include "bmp_reader.h"
#include <iostream>
#include <fstream>
#include <memory>

// Method for loading BMP file
bool BMP_File::Load_BMP_File(const char* file_name) {
    
    // Open file in reading mode
    std::ifstream file(file_name, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file: " << file_name << "\n";
        return false;
    }
    
    // Read BMP header and DIB header using reinterpret_cast
    file.read(reinterpret_cast<char*>(&bmp_header), sizeof(BMP_Header));
    file.read(reinterpret_cast<char*>(&dib_header), sizeof(DIB_Header));
    
    // Calculating pixel count and allocating memory for pixel data
    file_data = new RGB[dib_header.width * dib_header.height];
    
    // Row size (with padding) is aligned to 4 bytes
    int row_size = ((dib_header.width * dib_header.bits_per_pixel + 31) / 32) * 4;
    
    // Move file pointer to the beginning of pixel data
    file.seekg(bmp_header.pixel_offset, std::ios::beg);

    for (int y = dib_header.height - 1; y >= 0; --y) {
        
        // Read BMP File line to line
        file.read(reinterpret_cast<char*>(&file_data[y * dib_header.width]), dib_header.width * sizeof(RGB));
        
        // Skip padding bytes if any (BMP rows are padded to 4-byte boundaries)
        file.seekg(row_size - dib_header.width * sizeof(RGB), std::ios::cur);
    }

    file.close(); // Close file
    return true;
}

// Method for save new file
void BMP_File::Save_BMP_File(const char* output_filename) {

    // Open file
    std::ofstream file(output_filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Unable to open file for writing: " << output_filename << "\n";
        return;
    }
    
    // Copy header BMP and DIB in new file
    file.write(reinterpret_cast<const char*>(&bmp_header), sizeof(BMP_Header));
    file.write(reinterpret_cast<const char*>(&dib_header), sizeof(DIB_Header));
    
    file.seekp(dib_header.header_size + 14, std::ios::beg);

    // Colculate padding
    int row_size = ((dib_header.width * dib_header.bits_per_pixel + 31) / 32) * 4;
    uint32_t padding = row_size - (dib_header.width * sizeof(RGB));
    
    // Write pixels in file
    for (int y = dib_header.height - 1; y >= 0; --y) {
        file.write(reinterpret_cast<const char*>(&file_data[y * dib_header.width]), dib_header.width * sizeof(RGB));
        for (uint32_t p = 0; p < padding; ++p) {
            file.put(0);
        }
    }

    file.close(); // File close
    std::cout << "Image saved successfully to " << output_filename << "\n";
    
    
}

// Method for flip BMP file contra clockwise
std::unique_ptr<BMP_File> BMP_File::flip_BMP_90_contra_clockwise() {
    auto new_bmp_file = std::make_unique<BMP_File>();
    
    // Copy header in new BMP file
    new_bmp_file->bmp_header = bmp_header;
    new_bmp_file->dib_header = dib_header;
    
    // Swap width and height
    std::swap(new_bmp_file->dib_header.width, new_bmp_file->dib_header.height);

    new_bmp_file->file_data = new RGB[new_bmp_file->dib_header.width * new_bmp_file->dib_header.height];

    for (uint32_t y = 0; y < dib_header.height; ++y) {
        for (uint32_t x = 0; x < dib_header.width; ++x) {
        
            // Search old index pixel
            uint32_t old_index = y * dib_header.width + x;
            
            // Colculate new position for pixel
            uint32_t new_index = (dib_header.width - 1 - x) * new_bmp_file->dib_header.width + y;
            
            // Copy pixel on new position
            new_bmp_file->file_data[new_index] = file_data[old_index];
        }
    }

    return new_bmp_file;
}

// Method for flip BMP file clockwise
std::unique_ptr<BMP_File> BMP_File::flip_BMP_90_clockwise() {
    auto new_bmp_file = std::make_unique<BMP_File>();
    
    // Copy header in new BMP file
    new_bmp_file->bmp_header = bmp_header;
    new_bmp_file->dib_header = dib_header;
    
    // Swap width and height
    std::swap(new_bmp_file->dib_header.width, new_bmp_file->dib_header.height);

    new_bmp_file->file_data = new RGB[new_bmp_file->dib_header.width * new_bmp_file->dib_header.height];

    for (uint32_t y = 0; y < dib_header.height; ++y) {
        for (uint32_t x = 0; x < dib_header.width; ++x) {
        
            // Search old index pixel
            uint32_t old_index = y * dib_header.width + x;
            
            // Colculate new position for pixel
            uint32_t new_index = x * new_bmp_file->dib_header.width + (dib_header.height - 1 - y);
            
            // Copy pixel on new position
            new_bmp_file->file_data[new_index] = file_data[old_index];
        }
    }

    return new_bmp_file;
}
