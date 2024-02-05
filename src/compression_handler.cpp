#include "compression_handler.h"

namespace compression {

    std::string Compression_handler::remove_txt_extension(const std::string& file_name) {
        if (file_name.find(".txt") != std::string::npos) {
            return file_name.substr(0, file_name.find(".txt"));
        } else {
            return file_name;
        }
    }

    void Compression_handler::huff_compress(const std::string& file_name) {
        std::string file_no_extension = remove_txt_extension(file_name);

        // Write encoded file
        file.write_bits(file_no_extension + h_extension_compressed, hcoder.encode(file.open(file_no_extension + ".txt")));

        // Write frequency table
        file.write(file_no_extension + h_extension_freqtable, hcoder.return_freq_table_str(file.open(file_no_extension + ".txt")));
    }

    void Compression_handler::huff_decompress(const std::string &file_name) {
        std::string file_no_extension = remove_txt_extension(file_name);

        auto coded_text = file.open_bits(file_no_extension + h_extension_compressed, file_no_extension + h_extension_freqtable);
        auto freqtable = file.open(file_no_extension + h_extension_freqtable);
        file.write(file_no_extension + h_extension_decompressed, hcoder.decode(coded_text, freqtable));
    }

    void Compression_handler::lzw_compress(const std::string &file_name) {
        std::string file_no_extension = remove_txt_extension(file_name);

        file.write_bits(file_no_extension + lzw_extension_compressed, lzwcoder.encode(file.open(file_no_extension + ".txt")));
    }

    void Compression_handler::lzw_decompress(const std::string &file_name) {
        std::string file_no_extension = remove_txt_extension(file_name);

        auto coded_text_lzw = file.open_bits(file_no_extension+lzw_extension_compressed);
        auto decoded_text_lzw = lzwcoder.decode(coded_text_lzw);
        file.write(file_no_extension + lzw_extension_decompressed, decoded_text_lzw);
    }

}



