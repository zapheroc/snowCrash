#include <iostream>
#include "CImg.h"
#include <fstream>
#include <cstdlib> // for the exit function
#include <vector>
#include <math.h>

using namespace std;

void createSnowCrash(string fileName, string outputName = "snowCrash.png");
void extractSnowCrash(string fileName);
void parseArguments(int, char *[], string &, string &);

int main(int argc, char *argv[]) {

    if (argc > 2) {
    string inputFile = "//empty\\";
    string outputFile = "//empty\\";
    parseArguments(argc, argv, inputFile, outputFile);
        createSnowCrash(inputFile, outputFile);
    }
    else {
        extractSnowCrash(argv[1]);
    }
    return 0;
}

void extractSnowCrash(string fileName) {

    using namespace cimg_library;

    ofstream outData("decodedSnow.txt", ios::binary);
    CImg<unsigned char> img(fileName.c_str());
    char *ar = new char[3*img.height()*img.width()];
    for (unsigned int h = 0; h < img.height(); h++) {
        for (unsigned int w = 0; w < img.width(); w++) {
            outData << img(w, h, 2);
        }
    }
    delete[] ar;
}

void createSnowCrash(string fileName, string outputName) {

    using namespace cimg_library;

    ifstream inData(fileName, ios::binary|ios::ate);

    if (!inData) {
        cerr << "Input File was not found.";
        exit(1);
    }

    ifstream::pos_type pos = inData.tellg();
    vector<char> result(pos);
    inData.seekg(0, ios::beg);
    inData.read(result.data(), pos);

    unsigned int imageWidth = 0;
    unsigned int imageHeight = 0;
    unsigned int numberOfBytes = result.size();

    imageWidth = round(sqrt(numberOfBytes));

    if (imageWidth < 1)  {
        imageWidth = 1;
    }

    imageHeight = imageWidth;
    //Replace magic number 3 with RBG!
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
    img.save_png(outputName.c_str());
}

void parseArguments(int argc, char *argv[], string &inputFile, string &outputFile) {

    for (int arg = 0; arg < argc; arg++) {
        if (strcmp(argv[arg], "-i") == 0) {
            inputFile = argv[arg + 1];
        }
        else if (strcmp(argv[arg], "-o") == 0) {
            outputFile = argv[arg + 1];
        }
    }

    if (inputFile == "//empty\\" || outputFile == "//empty\\") {
        cerr << "No file given for input/output. Use -i for input, and -o for output" << endl;
        exit(1);
    }
}
