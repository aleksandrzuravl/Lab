#include "bmp_reader.h"


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
std::unique_ptr<BMP_File> BMP_File::flip_BMP_90_contra_clockwise(int thread_count) {
    auto new_bmp_file = std::make_unique<BMP_File>();

    // Copy headers and swap dimensions
    new_bmp_file->bmp_header = bmp_header;
    new_bmp_file->dib_header = dib_header;
    std::swap(new_bmp_file->dib_header.width, new_bmp_file->dib_header.height);

    new_bmp_file->file_data = new RGB[new_bmp_file->dib_header.width * new_bmp_file->dib_header.height];

    auto worker = [&](int start_row, int end_row) {
        for (int y = start_row; y < end_row; ++y) {
            for (uint32_t x = 0; x < dib_header.width; ++x) {
                uint32_t old_index = y * dib_header.width + x;
                uint32_t new_index = (dib_header.width - 1 - x) * new_bmp_file->dib_header.width + y;
                new_bmp_file->file_data[new_index] = file_data[old_index];
            }
        }
    };

    int total_rows = dib_header.height;
    int rows_per_thread = total_rows / thread_count;
    std::vector<std::thread> threads;

    for (int i = 0; i < thread_count; ++i) {
        int start_row = i * rows_per_thread;
        int end_row = (i == thread_count - 1) ? total_rows : start_row + rows_per_thread;
        threads.emplace_back(worker, start_row, end_row);
    }

    for (auto& t : threads) t.join();

    return new_bmp_file;
}

// Method for flip BMP file clockwise
std::unique_ptr<BMP_File> BMP_File::flip_BMP_90_clockwise(int thread_count) {
    auto new_bmp_file = std::make_unique<BMP_File>();

    // Copy headers and swap dimensions
    new_bmp_file->bmp_header = bmp_header;
    new_bmp_file->dib_header = dib_header;
    std::swap(new_bmp_file->dib_header.width, new_bmp_file->dib_header.height);

    new_bmp_file->file_data = new RGB[new_bmp_file->dib_header.width * new_bmp_file->dib_header.height];

    auto worker = [&](int start_row, int end_row) {
        for (int y = start_row; y < end_row; ++y) {
            for (uint32_t x = 0; x < dib_header.width; ++x) {
                uint32_t old_index = y * dib_header.width + x;
                uint32_t new_index = x * new_bmp_file->dib_header.width + (dib_header.height - 1 - y);
                new_bmp_file->file_data[new_index] = file_data[old_index];
            }
        }
    };

    int total_rows = dib_header.height;
    int rows_per_thread = total_rows / thread_count;
    std::vector<std::thread> threads;

    for (int i = 0; i < thread_count; ++i) {
        int start_row = i * rows_per_thread;
        int end_row = (i == thread_count - 1) ? total_rows : start_row + rows_per_thread;
        threads.emplace_back(worker, start_row, end_row);
    }

    for (auto& t : threads) t.join();

    return new_bmp_file;
}
