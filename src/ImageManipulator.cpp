#include "ImageManipulator.h"

using namespace std;

ImageManipulator::ImageManipulator()
{
    //ctor
}


void ImageManipulator::scaleImageForData( string imageName, float fileSize ) {

    using namespace cimg_library;

    img.load(imageName.c_str());

    //CImg<unsigned char> img(imageName.c_str());

    if ( (img.width() * img.height() ) <= fileSize ) {

    unsigned int finalHeight = ceil(sqrt((fileSize*img.height())/img.width()));
    unsigned int finalWidth = ceil(sqrt((fileSize*img.width())/img.height()));

    cout << " OWidth " << img.width() << " OHeight " << img.height() << endl;
    cout << " Width " << finalWidth << " height " << finalHeight << endl;
    img.resize(finalWidth, finalHeight);

    //img.save_png("scaledForData001.png");

    }
    else {
        cout << "Image did not need resizing: w*h=" << img.width() * img.height() << endl;
    }
}

void ImageManipulator::extractSnowCrash(string fileName, string outputFile) {

    using namespace cimg_library;

    ofstream outData(outputFile, ios::binary);

    CImg<unsigned char> img(fileName.c_str());
    for (int h = 0; h < img.height(); h++) {
        for (int w = 0; w < img.width(); w++) {
            if (img(w,h, 2) == '_') {
                const char *ar = reinterpret_cast<const char *>(img.data(w,h,0,2));
                if (strncmp(ar, END_OF_SNOWCRASH, strlen(END_OF_SNOWCRASH)) == 0) {
                    return;
                }
            }
            outData << img(w, h, 2);
        }
    }

}

void ImageManipulator::createSnowCrash(string fileName, string outputName) {

    using namespace cimg_library;

    //FINISH ADDING IN THE ID FOR THE FILENAME EXTRACTION LATER
    string id = "</_name_>";
    id = fileName + id;


    ifstream inData(fileName, ios::binary|ios::ate);

    if (!inData) {
        cerr << "Input File was not found.";
        exit(1);
    }

    ifstream::pos_type pos = inData.tellg();
    vector<char> result( static_cast<long>(pos) + strlen(END_OF_SNOWCRASH) );
    inData.seekg(0, ios::beg);
    inData.read(result.data(), pos);
    inData.close();
    result.insert(result.end() - strlen(END_OF_SNOWCRASH), END_OF_SNOWCRASH, END_OF_SNOWCRASH+strlen(END_OF_SNOWCRASH));

    unsigned int imageWidth = 0;
    unsigned int imageHeight = 0;
    unsigned int numberOfBytes = result.size();

    imageWidth = round(sqrt(numberOfBytes));

    if (imageWidth < 1)  {
        imageWidth = 1;
    }

    imageHeight = imageWidth;

    while (imageWidth * imageHeight < result.size()) {
        imageHeight++;
    }

    CImg<unsigned char> img(imageWidth, imageHeight, 1, 3);
    unsigned int arrayPos = 0;
    for (unsigned int h = 0; h < imageHeight; h++) {
        for (unsigned int w = 0; w < imageWidth; w++) {
        if (arrayPos < numberOfBytes) {
            img(w, h, 2) = result.at(arrayPos);
        }
        arrayPos++;
        }
    }
    cout << "Image creation complete. Now writing image to file format requested. " << endl;
    img.save_png(outputName.c_str());
}

void ImageManipulator::seedImage(vector<unsigned long> &randomPixelArray, vector<char> &dataFileVector) {

    unsigned char *pixel = img.data();
    cout << "Scaled image size: " << img.size() << " File to embed size: " << dataFileVector.size() << endl;
    for (unsigned int i = 0; i < dataFileVector.size(); i++) {
        *(pixel+randomPixelArray.at(i)) = dataFileVector.at(i);
    }
    cout << " loop finished " << endl;
    img.save_png("new_seeded_image_not_finished.png");
}

ImageManipulator::~ImageManipulator()
{
    //dtor
}
