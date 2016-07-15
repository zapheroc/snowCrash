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

void FileParser::generateRandomPixelArray() {

    const int vlength = dataFileVector.size();
    int *positions = new int[vlength];

    for (int i = 0; i < vlength; i++) {
        positions[i] = i;
    }

    cout << positions[0] << " " << positions[11] << " " << positions[vlength - 1] << endl;


}


/*
void FileParser::scaleImageForData( string imageName, float fileSize ) {

    using namespace cimg_library;
    CImg<unsigned char> img(imageName.c_str());

    if ( (img.width() * img.height() ) <= fileSize ) {

    unsigned int finalHeight = ceil(sqrt((fileSize*img.height())/img.width()));
    unsigned int finalWidth = ceil(sqrt((fileSize*img.width())/img.height()));

    cout << " OWidth " << img.width() << " OHeight " << img.height() << endl;
    cout << " Width " << finalWidth << " height " << finalHeight << endl;
    img.resize(finalWidth, finalHeight);
    img.save_png("scaledForData001.png");

    }
}
*/
FileParser::~FileParser()
{
    //dtor
}
