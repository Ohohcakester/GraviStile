#ifndef FILEIO_H_
#define FILEIO_H_

#include <fstream>
#include <string>

class FileIO {
public:
    std::ofstream stream;

    FileIO(std::string fileName);

    void writeLine(std::string line);
    void writeIndentedLine(int indentation, std::string line);
    void close();
};






#endif