#include <iostream>
#include "FileIO.h"

using namespace std;

FileIO::FileIO(string fileName) {
    stream.open(fileName);
}

void FileIO::close() {
    stream.close();
}