#include "SaveIO.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <limits.h>

using namespace std;

int CHECKSUM_LENGTH = 10;


SaveIO::SaveIO(int size) : size(size), expandedSize(size+CHECKSUM_LENGTH) {
    data.resize(size, false);
}

void SaveIO::save(std::string filename) {
    ofstream out(filename);
    appendChecksum(&data);
    permute(&data);

    std::vector<char> bitData;
    bitData.resize((expandedSize+CHAR_BIT-1)/CHAR_BIT, 0);
    for (int i = 0; i < expandedSize; ++i) {
        int bit = i % CHAR_BIT;
        int word = i / CHAR_BIT;

        bitData[word] |= data[i] << bit;
    }

    out << (char)size;
    for (int i = 0; i < bitData.size(); ++i) {
        out << bitData[i];
    }

    data.clear();
}

void SaveIO::load(std::string filename) {
    data.clear();
    data.resize(size, false);

    ifstream in(filename);
    int readSize = -1;

    std::vector<char> bitData;
    char c;
    while (in.get(c)) {
        if (readSize == -1) readSize = (int)c;
        else bitData.push_back(c);
    }

    if (readSize == -1 || readSize > bitData.size()*CHAR_BIT) return;

    data.resize(readSize + CHECKSUM_LENGTH, false);
    for (int i = 0; i < data.size(); ++i) {
        int bit = i % CHAR_BIT;
        int word = i / CHAR_BIT;

        data[i] = bitData[word] & (1 << bit);
    }

    unpermute(&data);
    checkAndRemoveChecksum(&data);
    data.resize(size, false);
}

bool generateChecksumValue(std::vector<bool>* vec, int size) {
    std::vector<bool>& v = *vec;

    int p = 31;
    if (size % 3 == 1) p = 23;
    if (size % 31 == 0) p = 17;
    int q = (size/3 + 2) * p + 3;

    bool b = false;
    int curr = size/2;
    for (int i = 0; i < q; ++i) {
        b ^= v[curr];
        curr = (curr + p) % size;
    }
    return b;
}

void SaveIO::appendChecksum(std::vector<bool>* vec) {
    std::vector<bool>& v = *vec;
    for (int i = 0; i < CHECKSUM_LENGTH; ++i) {
        v.push_back(generateChecksumValue(&v, v.size()));
    }
}

void SaveIO::checkAndRemoveChecksum(std::vector<bool>* vec) {
    std::vector<bool>& v = *vec;
    int originalSize = v.size() - CHECKSUM_LENGTH;
    for (int i = 0; i < CHECKSUM_LENGTH; ++i) {
        bool b = generateChecksumValue(&v, originalSize + i);
        if (v[originalSize + i] != b) {
            v.clear();
            v.resize(originalSize, false);
        }
    }
    v.resize(originalSize, false);
}

void swapBool(std::vector<bool>* vec, int p1, int p2) {
    bool temp = (*vec)[p1];
    (*vec)[p1] = (*vec)[p2];
    (*vec)[p2] = temp;
}

void SaveIO::permute(std::vector<bool>* vec) {
    std::vector<bool>& v = *vec;
    int len = v.size();

    int p1 = 9, p2 = 197;
    for (int i = 0; i < len; ++i) {
        if (v[i]) {
            p1 += 3;
            p2 += 2;
        }
    }
    p1 %= len;
    p2 %= len;

    for (int i = 0; i < 133; ++i) {
        swapBool(&v, p1, p2);
        p1 = (p1 + 31) % len;
        p2 = (p2 + 7) % len;
    }
}

void SaveIO::unpermute(std::vector<bool>* vec) {
    std::vector<bool>& v = *vec;
    int len = v.size();

    int p1 = 9, p2 = 197;
    for (int i = 0; i < len; ++i) {
        if (v[i]) {
            p1 += 3;
            p2 += 2;
        }
    }
    p1 %= len;
    p2 %= len;

    for (int i = 0; i < 133; ++i) {
        p1 = (p1 + 31) % len;
        p2 = (p2 + 7) % len;
    }

    for (int i = 0; i < 133; ++i) {
        p1 = ((p1 - 31) % len + len) % len;
        p2 = ((p2 - 7) % len + len) % len;
        swapBool(&v, p1, p2);
    }


}
