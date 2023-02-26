#include <iostream>
#include <fstream>
#include <zlib.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [input file]" << std::endl;
        return 1;
    }

    // Open the input file
    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file " << argv[1] << std::endl;
        return 1;
    }

    // Get the size of the input file
    input_file.seekg(0, std::ios::end);
    std::streampos input_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    // Allocate memory for the input buffer and read the input file into it
    char* input_buffer = new char[input_size];
    input_file.read(input_buffer, input_size);

    // Allocate memory for the output buffer
    uLong output_size = compressBound(input_size);
    Bytef* output_buffer = new Bytef[output_size];

    // Compress the input buffer into the output buffer
    int result = compress(output_buffer, &output_size, reinterpret_cast<const Bytef*>(input_buffer), input_size);
    if (result != Z_OK) {
        std::cerr << "Failed to compress input file " << argv[1] << std::endl;
        delete[] input_buffer;
        delete[] output_buffer;
        return 1;
    }

    // Compute compression ratio
    double compression_ratio = static_cast<double>(input_size) / static_cast<double>(output_size);

    // Print informative metrics
    std::cout << "Input file size: " << input_size << " bytes" << std::endl;
    std::cout << "Compressed file size: " << output_size << " bytes" << std::endl;
    std::cout << "Compression ratio: " << compression_ratio << std::endl;

    // Clean up
    delete[] input_buffer;
    delete[] output_buffer;

    return 0;
}
