#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <algorithm> // for std::shuffle
#include <boost/random/mersenne_twister.hpp>

// THIS DOES NOT ACTUALLY CREATE GOOD RANDOM NUMBERS
//#include <random> // for the uniform random number generators

#include <fstream> // for input and output streams
#include <vector> // for vectors containing data
#include <iostream> // for debugging. Remove later
#include <cstring> // check if this is needed
#include <cmath> // for sqrt()
#include "CImg.h"

class FileParser
{
    public:
        FileParser(std::string fileName);
        FileParser();
        virtual ~FileParser();
        float estimateImageSizeData( float ratioData );
        void generateRandomPixelArray(const cimg_library::CImg<unsigned char> &img, long seed);
        void longToBytes(unsigned long n, unsigned char ar[sizeof(unsigned long)]);
        unsigned long bytesToLong(unsigned char n[sizeof(unsigned long)]);
       // void scaleImageForData( std::string imageName, float fileSize );

        const char *END_OF_SNOWCRASH = "__!END!__!OF!__!SNOWCRASH!__";
        const int LONG_LENGTH = sizeof(unsigned long);
        std::vector<char> dataFileVector;
        std::vector<unsigned long> randomPixelArray;
        long fileSize;
    protected:
        //std::vector<int> randomPixelArray;
    private:
};

#endif // FILEPARSER_H
