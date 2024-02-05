#include "file_handler.h"
#include <bitset>

namespace files {

    std::string File_handler::open(const std::string &file_name) {
        file_in.open(file_name, std::ios::binary);
        std::string buffer{};

        if (!file_in) {
            // File has not been opened successfully
            throw std::runtime_error("Error opening file.");
        } else {
            // Read current contents into file_buffer
            while (file_in.peek() != EOF) {
                buffer.push_back((char) file_in.get());
            }

            file_in.close();
            return buffer;
        }
    }

    bool File_handler::write(const std::string &file_name, const std::string &to_write) {
        file_out.open(file_name, std::ios::binary);

        if (!file_out) {
            // File has not been created/opened successfully
            return false;
        } else {
            file_out << to_write; // Write buffer back into file
            file_out.close();
            return true;
        }

    }

    bool File_handler::write_bits(const std::string &file_name, const std::string &to_write) {
        file_out.open(file_name, std::ios::binary);
        std::string buffer = to_write;

        if (!file_out) {
            // File has not been created/opened successfully
            return false;
        } else {
            std::string output_chars{};


            // The buffer consists of a long string of bits like "101010100101001010101"
            // While the buffer isn't empty, pop out 8 bits at a time, convert this to a char representation
            // of the bits, and save this to the output string
            while (!buffer.empty()) {
                std::string temp(8, '0');

                // Put 8 bits in the string
                if (buffer.length() >= 8) {
                    for (int i{}; i < 8; i++) {

                        char c = buffer.front(); // Get character from the front
                        buffer.erase(buffer.begin()); // Delete character in the front

                        temp[i] = c;
                    }
                } else {
                    // If there are less than 8 bits left in the buffer, write them in and put all remaining bits to 0

                    // Length needs to be saved because elements will get deleted in subsequent runs through the for loop
                    // Therefore we can't use buffer.length() as the index condition
                    auto current_length = buffer.length();
                    for (int i{}; i < current_length; i++) {
                        char c = buffer.front(); // Get character from the front
                        buffer.erase(buffer.begin()); // Delete character in the front

                        temp[i] = c;
                    }
                }

                // Convert the temp_str from bits to a character and add it to output_chars string
                output_chars.push_back(binary_string_to_char(temp));
            }

            file_out << output_chars; // Write buffer back into file

            file_out.close();
            return true;
        }
    }

    char File_handler::binary_string_to_char(const std::string &binary_string) {
        int int_value = std::stoi(binary_string, nullptr, 2); // Convert to char
        return static_cast<char>(int_value);
    }

    std::string File_handler::open_bits(const std::string &file_name) {
        std::string file_chars = open(file_name);
        std::string file_binary{};

        // Convert characters to binary representing string
        for (unsigned long int i{}; i < file_chars.length(); i++) {
            int ascii = static_cast<int>(file_chars[i]);
            std::bitset<8> binary(ascii);

            file_binary.append(binary.to_string());
        }

        return file_binary;
    }

    std::string File_handler::open_bits(const std::string &file_name, const std::string &frequency_table_str) {
        std::string freq_table = open(frequency_table_str);


        std::string file_binary = open_bits(file_name);

        // Get total number of chars from the frequency table string to use as a cutoff for any extra zeroes at the end of the
        // last byte -> example: 01101101 1100  might have been saved as 01101101 11000000
        auto total_chars = std::stoi(freq_table.substr(freq_table.find('~') + 1));

        // Remove extra zeroes from the end
        file_binary.erase(total_chars, file_binary.length());

        return file_binary;
    }

}