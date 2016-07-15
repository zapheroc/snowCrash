#ifndef IMAGEMANIPULATOR_H
#define IMAGEMANIPULATOR_H

#include "CImg.h"


#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>


class ImageManipulator
{
    public:
        ImageManipulator();
        virtual ~ImageManipulator();

        void scaleImageForData( std::string imageName, float fileSize );
        void createSnowCrash( std::string fileName, std::string outputName);
        void extractSnowCrash( std::string fileName, std::string outputFile);

        cimg_library::CImg<unsigned char> img;
    protected:
    private:
        const char *END_OF_SNOWCRASH = "__!END!__!OF!__!SNOWCRASH!__";
};

#endif // IMAGEMANIPULATOR_H
