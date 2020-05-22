#include <iostream>
#include <map>
#include <math.h>
#include <assert.h>
#include <vector>
#include <bitset>
#include <algorithm>

// helper function for creating bitstrings from strings in hashtable

std::string string_to_bitstring(std::string str)    {
    std::string bin = "";

    for(char& c : str)  {
        std::string temp = std::bitset<8>(c).to_string();
        bin += temp;
    }

    return bin;
}

void add_to_table(std::map<std::string, int> &table, int val, std::string key)    {
    
    if(table.count(key) == 1)   {
        std::cout << "key: " << key << " has already been entered." << std::endl;
    }
    else    {
        table[string_to_bitstring(key)] = val;
    }
    
}
// populate table with ascii values
void populate_table(std::map<std::string, int> &table)   {

    for(int i = 0; i < 256; i++)   {
        std::bitset<8> temp(i);
        table[temp.to_string()] = i;
    }
    // todo: what do I set this to?
    table[""] = -2;

}

void send_magic()   {

    std::vector<unsigned char> numbers = {0x1f, 0x9d, 0x90};

    for(char c : numbers)   {
        std::cout.put(c);
    }
}

int search_table(std::map<std::string, int> &table, std::string value) {

    std::string bit_string = string_to_bitstring(value);
    
    if(table.count(bit_string) == 0 || table[bit_string] < 0)
        return -1;
    else
        return table[bit_string];
}

std::string encode_index(int i, int num_bits, std::vector<int> &sym_lens)  {
    
    std::bitset<16> temp(i);
    std::string bitstring = temp.to_string();
    // remove unneccesary bits
    while(bitstring.length() > num_bits)    {
        bitstring.erase(0,1);
    }    

    // add symbol lengths for creating bytes from bits
    sym_lens.push_back(bitstring.length());

    return bitstring;
}

std::string reverse_str(std::string str)    {
    reverse(str.begin(), str.end());
    return str;
}

void bits_to_bytes(std::string encoded, std::vector<int> sym_lens) {
    

    std::string en_bit_rev {""};
    // bit reversal of symbols
    int pos = 0;
    for(int len : sym_lens) {
        std::string sym = encoded.substr(pos, len);
        pos += len;
        en_bit_rev += reverse_str(sym);
    }

    // pad
    while(en_bit_rev.length()%8 != 0) {
        en_bit_rev += "0";
    }

    std::vector<std::bitset<8> > final_encoding {};
    pos = 0;
    while(pos < en_bit_rev.length())    {
        std::string byte = en_bit_rev.substr(pos, 8);
        pos += 8;
        std::bitset<8> b (reverse_str(byte));
        final_encoding.push_back(b);
    }

    assert (final_encoding.size() == en_bit_rev.length()/8);
    // std::cout << "final enc size: " << final_encoding.size() << std::endl;


    // send magic number and mode
    send_magic();
    // get bytes into output stream
    for(std::bitset<8> bits : final_encoding)    {
        unsigned long temp = bits.to_ulong();
        unsigned char c = static_cast<unsigned char>(temp);
        std::cout.put(c);
    }

}

int main(){

    // symbol table is indexed by the integer values. Corresponding are strings which represent bit vectors.
    std::map<std::string, int> symbol_table;
    populate_table(symbol_table);

    int num_bits = 9;
    int max_bits = 16;
    int next_symbol = 257;

    std::string encoded_output {""};
    std::vector<int> encoded_sym_lens {};

    std::string working {""};

//    LZW algorithm
    char c;
    while(std::cin.get(c)){

        std::string augmented {working + c};

        if(search_table(symbol_table, augmented) != -1) {
            working = augmented;
        }
        else if(next_symbol >= pow(2, max_bits))    {
            int i = search_table(symbol_table, working);

            if(i == -1) {
                std::cout << "working string not found in else if. exiting." << std::endl;
                return 1;
            }
            // encoded index
            encoded_output += encode_index(i, num_bits, encoded_sym_lens);
            working = c;
        }
        else    {

            add_to_table(symbol_table, next_symbol, augmented);
            int i = search_table(symbol_table, working);

            if(i == -1) {
                std::cout << "working string not found in else. exiting." << std::endl;
                return 1;
            }
            // encoded index
            encoded_output += encode_index(i, num_bits, encoded_sym_lens);

            working = c;
            next_symbol += 1;

            if(next_symbol > pow(2, num_bits))
                num_bits +=1;
        }       
    }

    if(working.length() > 0)    {
        int i = search_table(symbol_table, working);
        // todo output encoded index
        encoded_output += encode_index(i, num_bits, encoded_sym_lens);
    }

    bits_to_bytes(encoded_output, encoded_sym_lens);

    return 0;
}



