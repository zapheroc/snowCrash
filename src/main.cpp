#include <iostream>
#include "CImg.h"
#include <fstream>
#include <cstdlib> // for the exit function
#include <vector>
#include <math.h>
#include <getopt.h>
#include "FileParser.h"
#include "ImageManipulator.h"

using namespace std;

const char *END_OF_SNOWCRASH = "__!END!__!OF!__!SNOWCRASH!__";
const string VERSION = "1.0";
const string EMPTY_STRING = "//empty\\";

void createSnowCrash(string fileName, string outputName = "snowCrash.png");
void extractSnowCrash(string fileName, string outputFile);

int main(int argc, char *argv[]) {

    int opt;
    string inputFile = EMPTY_STRING;
    string outputFile = EMPTY_STRING;
    string extractFile = EMPTY_STRING;

    while ((opt = getopt(argc, argv, "i:o:e:vh")) != -1) {
        switch (opt) {
            case 'i':
                inputFile = optarg;
                break;
            case 'o':
                outputFile = optarg;
                break;
            case 'e':
                extractFile = optarg;
                break;
            case 'v':
                cout << "SnowCrash Version " << VERSION << endl;
                return 0;
            case 'h':
                cout << "Use -e to extract, -i to input, and -o to specify output for both." << endl;
                return 0;
            default:
                cerr << "No required options specified" << endl;
        }
    }

    /* CHECK IF FILE ALREADY EXISTS TO PREVENT DESTROYING USERS FILES */
    ifstream does_exist(outputFile);
    if (does_exist) {
        string response = "";
        cout << "File exists. Overwrite? (y/n): ";
        cin >> response;
        if (response != "y" && response != "Y") {
            cerr << "Aborted " << endl;
            exit(1);
        }
    }
    does_exist.close();

    FileParser *testFileData = new FileParser(inputFile);
    cout << "The file size is " << testFileData->fileSize << " bytes \n" << "The first byte of data " << testFileData->dataFileVector.at(0) << endl;
    cout << "Required number of pixels for data given: " << testFileData->estimateImageSizeData(1.0) << endl;
    float fileSize = testFileData->estimateImageSizeData(0.35f);
    cout << "File Size at given ratio: " << fileSize << endl;
    string ss = "Rame.JPG";

    ImageManipulator *testManipulator = new ImageManipulator();

    testManipulator->scaleImageForData(ss, fileSize);
    testFileData->generateRandomPixelArray(testManipulator->img, 20);
    testManipulator->seedImage(testFileData->randomPixelArray, testFileData->dataFileVector);

    //testFileData->scaleImageForData(ss, sz);

    /*
    if (inputFile != EMPTY_STRING && outputFile != EMPTY_STRING) {
        createSnowCrash(inputFile, outputFile);
    }
    else if (extractFile != EMPTY_STRING && outputFile != EMPTY_STRING) {
        extractSnowCrash(extractFile, outputFile);
    }
    else {
        cerr << "Missing required flags for operation. Use -e to extract, -i to input, and -o to specify output for both" << endl;
    }
    return 0;
    */
}

void extractSnowCrash(string fileName, string outputFile) {

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

void createSnowCrash(string fileName, string outputName) {

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
