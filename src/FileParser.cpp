#include "FileParser.h"
#include "CImg.h"

using namespace std;

FileParser::FileParser(string fileName )
{
   ifstream inData(fileName, ios::binary|ios::ate);

    if (!inData) {
        cerr << "Input File was not found.";
        exit(1);
    }

    ifstream::pos_type pos = inData.tellg();
    dataFileVector.resize( static_cast<long>(pos) + strlen(END_OF_SNOWCRASH) );
    inData.seekg(0, ios::beg);
    inData.read(dataFileVector.data(), pos);
    inData.close();
    dataFileVector.insert(dataFileVector.end() - strlen(END_OF_SNOWCRASH), END_OF_SNOWCRASH, END_OF_SNOWCRASH+strlen(END_OF_SNOWCRASH));
    fileSize = dataFileVector.size();
}

float FileParser::estimateImageSizeData( float ratioData = 1.0) {

    if (ratioData > 1 || ratioData < 0) {
        cerr << "The ratio was out of range. Use decimal values between 1 and 0" << endl;
        exit(1);
    }
    return (fileSize / ratioData) / 3;
}

void FileParser::generateRandomPixelArray(const cimg_library::CImg<unsigned char> &img, long seed ) {
    using namespace cimg_library;

    int imgSize = img.size() - 1;

    randomPixelArray.resize(imgSize);

    for (unsigned int i = 0; i < imgSize; i++) {
        randomPixelArray[i] = i;
    }

    mt19937 engine(seed);
    shuffle(randomPixelArray.begin(), randomPixelArray.end(), engine);
    randomPixelArray.resize(dataFileVector.size());
}

FileParser::~FileParser()
{
    //dtor
}
