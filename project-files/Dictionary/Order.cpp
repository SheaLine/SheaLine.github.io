// ----------------------------------------------------------------------------
// Shea Line
// sline
// 2023 Winter CSE101 pa8
// ----------------------------------------------------------------------------
#include <fstream>
#include <iostream>
#include <string>
#include "Dictionary.h"

//using namespace std;


int main(int argc, char* argv[]) 
{
    // Check command line arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
        return 1;
    }

    // Create an instance of the Dictionary
    Dictionary dict;

    // Open the input file
    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Unable to open input file\n";
        return 1;
    }

    // Read lines from the input file and insert them into the Dictionary
    std::string line;
    int lineNumber = 1;
    while (std::getline(inputFile, line)) {
        dict.setValue(line, lineNumber);
        lineNumber++;
    }

    // Close the input file
    inputFile.close();

    // Open the output file
    std::ofstream outputFile(argv[2]);
    if (!outputFile) {
        std::cerr << "Unable to open output file\n";
        return 1;
    }

    // Write the first string representation to the output file
    outputFile << dict.to_string();

    outputFile << std::endl;

    // Write the second string representation to the output file
    outputFile << dict.pre_string();

    // Close the output file
    outputFile.close();

    return 0;
}