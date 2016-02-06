#ifndef SAVE_IO_H_
#define SAVE_IO_H_

#include <bitset>
#include <iostream>
#include <vector>

// NOTE: CAN SAVE A MAXIMUM OF 127 BOOLS.
class SaveIO {
public:
    std::vector<bool> data;
    const int size;
    const int expandedSize;

    SaveIO(int size);

    void save(std::string filename);
    void load(std::string filename);

    void appendChecksum(std::vector<bool>* v);
    void checkAndRemoveChecksum(std::vector<bool>* v);
    void permute(std::vector<bool>* vec);
    void unpermute(std::vector<bool>* vec);
};

#endif