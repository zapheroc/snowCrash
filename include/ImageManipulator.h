#ifndef IMAGEMANIPULATOR_H
#define IMAGEMANIPULATOR_H

#include "CImg.h"
#include "FileParser.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>


class ImageManipulator
{
    public:
        ImageManipulator();
        ImageManipulator(std::string imageName);
        virtual ~ImageManipulator();

        void extractSeededImage( std::string outputFile, std::seed_seq &seed);
        void scaleImageForData( float fileSize );
        void createSnowCrash( std::string fileName, std::string outputName);
        void extractSnowCrash( std::string fileName, std::string outputFile);
        void seedImage(std::vector<unsigned long> &randomPixelArray, std::vector<char> &dataFileVector, std::string outputFile);
        void addNoise(double sigma, unsigned int type);


        cimg_library::CImg<unsigned char> img;
    protected:
    private:
        const char *END_OF_SNOWCRASH = "__!END!__!OF!__!SNOWCRASH!__";
};

#endif // IMAGEMANIPULATOR_H
