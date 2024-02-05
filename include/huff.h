#ifndef COMPRESSION_HUFF_H
#define COMPRESSION_HUFF_H

#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <map>

namespace huff {

    // Reverse keys and values of a map
    template<typename T, typename U>
    static std::map<U, T> reverse_map(const std::map<T, U> &og_map) {
        std::map<U, T> temp_map;
        for (const auto &it: og_map)
            temp_map[it.second] = it.first;
        return temp_map;
    }

    // Min heap node in the huffman tree
    struct Node {
        int freq;
        char data;
        Node *left{nullptr};
        Node *right{nullptr};

        Node(char data, int freq) {
            this->freq = freq;
            this->data = data;
        }
    };

    // Comparison object to be used to order the priority queue
    // This struct is passed to std::priority_queue to change the ordering
    struct Node_compare {
        bool operator()(Node *l, Node *r) {
            // highest priority item has lowest frequency
            return l->freq > r->freq;
        }
    };

    // Builds the tree from a passed frequency table and outputs the coding table of the tree
    class Tree {
    public:

        void add_freq_table(const std::vector<Node> &n); // Create tree out of the added frequency table
        std::map<char, std::string> return_coding_table(); // Returns coding table of each character by walking down the tree
        void print_debug_tree(); // Print tree

    private:

        // Hold pointers to the current node's root, left and right node, used for building/navigating the tree
        Node *root{}, *left{}, *right{};

        // Min priority queue (min heap) used for building and storing the Huffman tree
        std::priority_queue<Node *, std::vector<Node *>, Node_compare> min_heap{};

        void build_tree(); // Builds the huffman inside the min heap with the current nodes

        // The following functions recursively navigate the tree to print it or create the coding table
        // usage: print_from_node(root node to walk down from, empty string to add the recursive output to)
        // usage: coding_table_from_node(root node to walk down from, empty string to add the recursive output to, coding table map to fill with coding information)
        void print_from_node(Node *n, const std::string& code_recursive);

        void coding_table_from_node(Node *n, const std::string& code_recursive, std::map<char, std::string> &coding_table);
        void clear_min_heap(); // Clear the contents of the tree

    };

    // Encodes and decodes text_strings
    // Encoding: convert a text_string into a frequency table, passes this to Tree to build the tree and get the coding table, then encodes the message with the coding table.
    // Decoding: passes an encoded_text_string with frequency table to Tree to build the tree and get the coding table, then decodes the message with the coding table.
    class Huffman_coder {
    public:
        std::string encode(const std::string &text_str); // Encode passed text_str and return the encoded text_str
        std::string return_freq_table_str(const std::string &text_str); // Returns the freq_table_str for the passed string which is required metadata for decoding

        std::string decode(const std::string &encoded_text_str,
                           const std::string &freq_table_str); // returns decoded encoded_text_str of the passed encoded_text_str and freq_table_str

    private:
        Tree tree{};

        std::string code_with_coding_table(const std::string &text_str,
                                           const std::map<char, std::string> &coding_table);

        std::string decode_with_coding_table(std::string encoded_text_str,
                                             const std::map<char, std::string> &coding_table);

        // Conversions
        // freq table with datatype std::vector<Node> is only used internally. For exporting, the freq table is translated into a freq_table_str
        std::vector<Node> string_to_nodes(const std::string &text_str); // Returns a frequency-sorted Node vector of a pair of each character and its frequency
        std::vector<Node> freq_table_str_to_nodes(std::string freq_table_str); // Converts freq_table_str to a freq_table: std::vector<Node>
        std::string nodes_to_freq_table_str(const std::vector<Node> &nodes); // Converts freq table: std::vector<Node> into freq_table_str


        static bool cmp_map_sort(std::pair<char, int> &a, std::pair<char, int> &b);
    };

}

#endif //COMPRESSION_HUFF_H