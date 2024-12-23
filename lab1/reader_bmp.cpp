#include "reader_bmp.h"
#include <iostream>
#include <fstream>
#include <memory>

bool BMP_File::Load_image(const char* f_name){
    // open file in reading mode
    std::ifstream file(f_name, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file: " << f_name << "\n";
        return false;
    }
    
    // read BMP header and DIB header using reinterpret_cast
    file.read(reinterpret_cast<char*>(&bmp_header), sizeof(BMP_Header));
    file.read(reinterpret_cast<char*>(&dib_header), sizeof(DIB_Header));
    
    
    file_data = new RGB[dib_header.width * dib_header.height];
    // row size (with padding) is aligned to 4 bytes
    int row_size = ((dib_header.width * dib_header.bits_per_pixel + 31) / 32) * 4;
    
    // move file pointer to the beginning of pixel data
    file.seekg(bmp_header.pixel_offset, std::ios::beg);

    for (int y = dib_header.height - 1; y >= 0; --y) {
        
        // read BMP file line to line
        file.read(reinterpret_cast<char*>(&file_data[y * dib_header.width]), dib_header.width * sizeof(RGB));
        
        // Skip padding bytes if any (BMP rows are padded to 4-byte boundaries)
        file.seekg(row_size - dib_header.width * sizeof(RGB), std::ios::cur);
    }

    file.close(); // Close file
    return true;
}

void BMP_File::Save_image(const char* output_f_name) {

    // open file
    std::ofstream file(output_f_name, std::ios::binary);
    if (!file)
    {
        std::cerr << "Unable to open file for writing: " << output_f_name << "\n";
        return;
    }
    
    // copy header BMP and DIB in new file
    file.write(reinterpret_cast<const char*>(&bmp_header), sizeof(BMP_Header));
    file.write(reinterpret_cast<const char*>(&dib_header), sizeof(DIB_Header));
    
    file.seekp(dib_header.header_size + 14, std::ios::beg);

    // colculate padding
    int row_size = ((dib_header.width * dib_header.bits_per_pixel + 31) / 32) * 4;
    uint32_t padding = row_size - (dib_header.width * sizeof(RGB));
    
    // write pixels in file
    for (int y = dib_header.height - 1; y >= 0; --y) {
        file.write(reinterpret_cast<const char*>(&file_data[y * dib_header.width]), dib_header.width * sizeof(RGB));
        for (uint32_t p = 0; p < padding; ++p) {
            file.put(0);
        }
    }

    file.close(); // file close
    std::cout << "saved " << output_f_name << "\n";
    
    
}

// rotate BMP file contra clockwise
std::unique_ptr<BMP_File> BMP_File::rotate_contra_clockwise() {
    auto nf_BMP = std::make_unique<BMP_File>();
    
    // copy header in new BMP file
    nf_BMP->bmp_header = bmp_header;
    nf_BMP->dib_header = dib_header;
    
    // swap width and height
    std::swap(nf_BMP->dib_header.width, nf_BMP->dib_header.height);

    nf_BMP->file_data = new RGB[nf_BMP->dib_header.width * nf_BMP->dib_header.height];

    for (uint32_t y = 0; y < dib_header.height; ++y) {
        for (uint32_t x = 0; x < dib_header.width; ++x) {
        
            // search old index pixel
            uint32_t old_index = y * dib_header.width + x;
            
            // colculate new position for pixel
            uint32_t new_index = (dib_header.width - 1 - x) * nf_BMP->dib_header.width + y;
            
            // copy pixel on new position
            nf_BMP->file_data[new_index] = file_data[old_index];
        }
    }

    return nf_BMP;
}

// rotate BMP file clockwise
std::unique_ptr<BMP_File> BMP_File::rotate_clockwise() {
    auto nf_BMP = std::make_unique<BMP_File>();
    
    // copy header in new BMP file
    nf_BMP->bmp_header = bmp_header;
    nf_BMP->dib_header = dib_header;
    
    // swap width and height
    std::swap(nf_BMP->dib_header.width, nf_BMP->dib_header.height);

    nf_BMP->file_data = new RGB[nf_BMP->dib_header.width * nf_BMP->dib_header.height];

    for (uint32_t y = 0; y < dib_header.height; ++y) {
        for (uint32_t x = 0; x < dib_header.width; ++x) {
        
            // search old index pixel
            uint32_t old_index = y * dib_header.width + x;
            
            // colculate new position for pixel
            uint32_t new_index = x * nf_BMP->dib_header.width + (dib_header.height - 1 - y);
            
            // copy pixel on new position
            nf_BMP->file_data[new_index] = file_data[old_index];
        }
    }

    return nf_BMP;
}
