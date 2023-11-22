// ----------------------------------------------------------------------------
// Shea Line
// sline
// 2023 Winter CSE101 pa8
// ----------------------------------------------------------------------------

#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include<algorithm>
#include "Dictionary.h"

using namespace std;

#define MAX_LEN 300

int main(int argc, char * argv[]){

    ifstream in;
    ofstream out;
    string line;
    string token;
    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";

    // check command line for correct number of arguments
    if( argc != 3 ){
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    // open files for reading and writing 
    in.open(argv[1]);
    if( !in.is_open() ){
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if( !out.is_open() ){
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }

    Dictionary wordDict;

    // read each line of input file, parse words and count frequencies
    while(getline(in, line)){
        size_t pos = 0;
        while((pos = line.find_first_not_of(delim, pos)) != string::npos){
            size_t end = line.find_first_of(delim, pos);
            if(end == string::npos)
                end = line.length();
            token = line.substr(pos, end-pos);

            // convert to lower case
            transform(token.begin(), token.end(), token.begin(), ::tolower);
            
            if(wordDict.contains(token)) // If word exists, increment count
                wordDict.setValue(token, wordDict.getValue(token)+1);
            else // Otherwise add new word with count 1
                wordDict.setValue(token, 1);

            pos = end + 1;
        }
    }

    // write the word frequencies to output file
    out << wordDict.to_string();

    // close files 
    in.close();
    out.close();

    return(EXIT_SUCCESS);
}