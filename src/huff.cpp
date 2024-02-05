#include "huff.h"
#include <algorithm>

/*************************************************************************************************************************
 *  Implementations of build_tree() and the recursive pathing functions inspired by GeeksforGeeks Huffman Coding tutorial:
 *  https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
 ************************************************************************************************************************/

namespace huff {

    bool Huffman_coder::cmp_map_sort(std::pair<char, int> &a, std::pair<char, int> &b) {
        return a.second < b.second;
    }

    void Tree::add_freq_table(const std::vector<Node> &n) {
        // Wipe current contents of min heap (tree)
        clear_min_heap();

        // Add new freq_table to min_heap
        for (auto i: n) {
            min_heap.push(new Node(i.data, i.freq));
        }
        build_tree();
    }

    void Tree::build_tree() {
        // Iterate while heap has loose nodes
        while (min_heap.size() > 1) {
            // Extract the 2 lowest freq items from heap
            left = min_heap.top();
            min_heap.pop();

            right = min_heap.top();
            min_heap.pop();

            // Add a new root node to these two nodes with the frequency being the sum of the frequency of both left and right children
            // '~' character reserved for indicating the root node
            root = new Node('~', left->freq + right->freq);
            root->left = left;
            root->right = right;

            // Push new tree part back into the min heap
            min_heap.push(root);
        }
    }

    void Tree::print_from_node(Node *n, const std::string& code_recursive) {

        // If null, we've reached the end, break from recursion
        if (n == nullptr) {
            return;
        }

        // Only print if it's not a root node
        if (n->data != '~') {
            std::cout << n->data << ": " << code_recursive << "\n";
        }

        print_from_node(n->left, code_recursive + "0"); // Left child, add a 0 to the code
        print_from_node(n->right, code_recursive + "1"); // Right child, add 1 to the code
    }

    void Tree::coding_table_from_node(Node *n, const std::string& code_recursive, std::map<char, std::string> &coding_table) {
        // If null, we've reached the end, break from recursion
        if (n == nullptr) {
            return;
        }

        // Only output if it's not a root node
        if (n->data != '~') {
            coding_table.insert(std::make_pair(n->data, code_recursive));
        }

        coding_table_from_node(n->left, code_recursive + "0", coding_table); // Left child, add a 0 to the code
        coding_table_from_node(n->right, code_recursive + "1", coding_table); // Right child, add 1 to the code
    }

    void Tree::print_debug_tree() {
        print_from_node(root, "");
    }

    std::map<char, std::string> Tree::return_coding_table() {
        std::map<char, std::string> temp_coding_table{};
        coding_table_from_node(root, "", temp_coding_table);

        return temp_coding_table;
    }

    void Tree::clear_min_heap() {
        while (!min_heap.empty()) {
            min_heap.pop();
        }
    }

    std::vector<Node> Huffman_coder::string_to_nodes(const std::string &text_str) {
        /// Count the frequency of each character, output map is sorted alphabetically first

        std::map<char, int> freq_map; // Holds a map of each character and its frequency, sorted alphabetically
        std::vector<std::pair<char, int>> freq_sorted{}; // Sorted by-frequency vector copy of the map
        std::vector<Node> nodes{}; // Vector of nodes (frequency table) that gets returned

        // Traverse the string
        for (int i{0}; text_str[i]; i++) {
            // If the current character hasn't been found before, set frequency to 1
            if (freq_map.find(text_str[i]) == freq_map.end()) {
                freq_map.insert(std::make_pair(text_str[i], 1));
            } else {
                // If the character already exists in the map, increase the frequency
                freq_map[text_str[i]]++;
            }
        }

        /// Now re-sort by frequency
        // Copy map into vector
        for (auto &it: freq_map) {
            freq_sorted.push_back(it);
        }

        // Sort by frequency
        std::sort(freq_sorted.begin(), freq_sorted.end(), cmp_map_sort);

        /// Convert sorted vector to nodes and add them to nodes vector
        for (auto &it: freq_sorted) {
            Node temp(it.first, it.second);
            nodes.push_back(temp);
        }

        return nodes;
    }

    std::string Huffman_coder::code_with_coding_table(const std::string &text_str,
                                                      const std::map<char, std::string> &coding_table) {

        std::string coded{};

        for (auto i: text_str) {
            coded.append(coding_table.find(i)->second);
        }

        return coded;
    }

    std::string Huffman_coder::decode_with_coding_table(std::string encoded_text_str,
                                                        const std::map<char, std::string> &coding_table) {

        std::string read_buffer{};
        std::string output_string{};
        auto reversed_table = reverse_map(coding_table);

        // We will copy an element from encoded_text_str to read_buffer, then remove it from the encoded_text_str
        // Read_buffer will be checked to the coding table:
        // If the contents of read_buffer are equal to the code of a character from the coding table,
        // then the corrresponding character will be added to the output string and read_buffer will be wiped
        // This will continue until encoded_text_str is empty;

        // add a code number to the buffer until it is something that exists inside the coding table
        while (!encoded_text_str.empty()) {
            read_buffer.push_back(encoded_text_str.front()); // Add front element of encoded text to read buffer

            encoded_text_str.erase(encoded_text_str.begin()); // delete front element from encoded text

            // Check if the code contents of read buffer correspond to a character in the map
            // map.find() returns the end iterator if nothing is found

            // If the value exists
            if (reversed_table.find(read_buffer) != reversed_table.end()) {

                // add the corresponding character of the code to the output string
                output_string.push_back(reversed_table.find(read_buffer)->second);

                // Wipe the read_buffer
                read_buffer.clear();
            }
        }

        if (!read_buffer.empty()) {
            throw std::runtime_error("read_buffer wasn't empty, therefore a character was in the decoding string that wasn't encoded in the coding table!");
        }

        return output_string;
    }

    std::string Huffman_coder::encode(const std::string &text_str) {
        auto freq_table = string_to_nodes(text_str);
        tree.add_freq_table(freq_table);
        auto coding_table = tree.return_coding_table();
        //tree.print_debug_tree();

        // Encode message with coding table
        auto coded_message = code_with_coding_table(text_str, coding_table);

        // Return encoded message
        return coded_message;
    }

    std::string Huffman_coder::return_freq_table_str(const std::string &text_str) {
        auto freq_table_str = nodes_to_freq_table_str(string_to_nodes(text_str));

        // Append the number of total bits that were in the original coded message to the end
        // This is needed for decoding later.
        auto coded_message = encode(text_str);
        freq_table_str.append("~" + std::to_string(coded_message.length()) + '|');

        return freq_table_str;
    }

    std::string Huffman_coder::decode(const std::string &encoded_text_str, const std::string &freq_table_str) {

        auto freq_table = freq_table_str_to_nodes(freq_table_str);

        tree.add_freq_table(freq_table);
        auto coding_table = tree.return_coding_table();
        //tree.print_debug_tree();

        // Decode message with coding table
        auto decoded_message = decode_with_coding_table(encoded_text_str, coding_table);

        // Return encoded message
        return decoded_message;
    }

    std::string Huffman_coder::nodes_to_freq_table_str(const std::vector<Node> &nodes) {
        // NOTE: THE OUTPUT OF THIS FUNCTION DOES NOT INCLUDE THE TOTAL LENGTH OF THE CODED MESSAGE
        // NECESSARY FOR DECODING, THIS IS DONE IN return_freq_table_str

        std::string freq_table_str{}; // To save the translated string in

        // Walk through the nodes and append the character and its frequency to the string
        // '|' character is reserved for subdividing the frequency table!
        for (int i{}; i < nodes.size(); i++) {
            std::string data_char{nodes[i].data};
            freq_table_str.append(data_char + std::to_string((int) nodes[i].freq) + '|');
        }

        return freq_table_str;
    }

    std::vector<Node> Huffman_coder::freq_table_str_to_nodes(std::string freq_table_str) {
        std::string temp_extracted_str{};
        std::vector<Node> nodes{};

        // While the freq_table_str hasn't been fully read out yet
        while (!freq_table_str.empty()) {

            // Read line until it hits '|' and put it into temp_extracted_str
            // This means '|' character is reserved!
            temp_extracted_str = freq_table_str.substr(0, freq_table_str.find('|'));

            // Delete extracted part from the string, + 1 to also delete the '|'
            freq_table_str.erase(0, temp_extracted_str.length() + 1);

            // Extract data and frequency

            char node_char = temp_extracted_str.front(); // First position is the character of the node
            temp_extracted_str.erase(temp_extracted_str.begin()); // Delete the first position
            int node_freq = std::stoi(temp_extracted_str); // Rest of the characters is the freq value

            // Convert into node and push into node vector if it's not '~'
            // Note: '~' is reserved for the internal root character in the tree, and reserved for indicating the
            // total number of bits inside the encoded string. Therefore, we throw it away, so it doesn't end up
            // in the tree.
            if (node_char != '~') {
                nodes.push_back(Node(node_char, node_freq)); // Add node with node_char and node_freq to the vector
            }


            // Clear the rest of temp_extracted_str;
            temp_extracted_str.clear();
        }

        return nodes;
    }


}