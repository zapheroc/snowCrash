#include "ImageManipulator.h"

using namespace std;

//For pure extraction, and image is not needed as a parameter.
ImageManipulator::ImageManipulator()
{
    //ctor
}

//On creation, this object should take an image as a parameter.
ImageManipulator::ImageManipulator(string imageName)
{
    img.load(imageName.c_str());
}

void ImageManipulator::scaleImageForData(float fileSize ) {

    using namespace cimg_library;

    if ( (img.width() * img.height() ) <= fileSize ) {

    unsigned int finalHeight = ceil(sqrt((fileSize*img.height())/img.width()));
    unsigned int finalWidth = ceil(sqrt((fileSize*img.width())/img.height()));

    cout << "Original Width: " << img.width() << " Original Height: " << img.height() << endl;
    cout << "Final Width: " << finalWidth << " Final Height: " << finalHeight << endl;
    img.resize(finalWidth, finalHeight);

    }
    else {
        cout << "Image did not need resizing: w*h=" << img.width() * img.height() << endl;
    }
}

//New method of seeding random image non-linearly. Takes a randomized sequential array, which represents random pixels in an image, and a data vector containing file data.
void ImageManipulator::seedImage(vector<unsigned long> &randomPixelArray, vector<char> &dataFileVector, string outputFile) {

    unsigned char *pixel = img.data();
    cout << "Scaled image size: " << img.size() << " File to embed size: " << dataFileVector.size() << endl;
    for (unsigned int i = 0; i < dataFileVector.size(); i++) {
        *(pixel+randomPixelArray.at(i)) = dataFileVector.at(i);
    }

    cout << "Seeding loop finished." << endl;
    img.save_png(outputFile.c_str());
}

//This function will take an output file name, and a seed, and extract an image based on those inputs.
void ImageManipulator::extractSeededImage(string outputFile, seed_seq &seed) {
    ofstream outData(outputFile, ios::binary);
    unsigned char *pixel = img.data();

    FileParser *fileExtractor = new FileParser;

    fileExtractor->generateRandomPixelArray(img, seed);

    unsigned char fileSizeAsArrayOfBytes[fileExtractor->LONG_LENGTH];

    for (int i = 0; i < fileExtractor->LONG_LENGTH; i++) {
        fileSizeAsArrayOfBytes[i] = *(pixel+(fileExtractor->randomPixelArray.at(i)));
        //cout << "Array Element for original size: " << static_cast<int>(fileSizeAsArrayOfBytes[i]) << endl;
    }

    unsigned long fileLength = fileExtractor->bytesToLong(fileSizeAsArrayOfBytes);
    cout << "File Length as extracted: " << fileLength << endl;

    for (unsigned long pix = 0; pix < fileLength; pix++) {
        outData << *(pixel+(fileExtractor->randomPixelArray.at(pix+fileExtractor->LONG_LENGTH)));
    }
}

void ImageManipulator::addNoise(double sigma, unsigned int type) {

    cout << "Adding noise." << endl;
    if (type > 4) {
        type = 4;
    }

    img.noise(sigma, type);
}

ImageManipulator::~ImageManipulator()
{
    //dtor
}
