#include <iostream>
#include "CImg.h"
#include <fstream>
#include <cstdlib> // for the exit function
#include <vector>
#include <math.h>
#include <getopt.h>
#include "FileParser.h"
#include "ImageManipulator.h"
#include <stdlib.h>

using namespace std;

const char *END_OF_SNOWCRASH = "__!END!__!OF!__!SNOWCRASH!__";
const string VERSION = "1.0";
const string EMPTY_STRING = "//empty\\";

void createSnowCrash(string fileName, string outputName = "snowCrash.png");
void extractSnowCrash(string fileName, string outputFile);

int main(int argc, char *argv[]) {

    int opt;
    string inputImage = EMPTY_STRING;
    string inputData = EMPTY_STRING;
    string outputFile = EMPTY_STRING;
    string extractFile = EMPTY_STRING;
    int noiseType = 0;
    long seed = 20;
    float percent = 1;
    double noise = 0;
    bool addNoise = false;


    // Process the input arguments, and store them in the appropriate variables.
    while ((opt = getopt(argc, argv, "d:i:n:o:s:t:p:vh")) != -1) {
        switch (opt) {
            case 's':
                seed = atoi(optarg);
                cout << seed << " seed " << endl;
                break;
            case 'p':
                percent = atof(optarg);
                cout << percent << " precent" << endl;
                break;
            case 'i':
                inputImage = optarg;
                break;
            case 'd':
                inputData = optarg;
                break;
            case 'n':
                addNoise = true;
                noise = atof(optarg);
            case 'o':
                outputFile = optarg;
                break;
            case 't':
                noiseType = atoi(optarg);
                break;
            case 'v':
                cout << "SnowCrash Version " << VERSION << endl;
                return 0;
            case 'h':
                cout << "Use -e to extract, -i to input, and -o to specify output, -s to specify seed, default is 20, -p to specify image density, default is 1." << endl;
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

    //If you have an inpute, output, and data, then create a new file
    if (inputImage != EMPTY_STRING && outputFile != EMPTY_STRING && inputData != EMPTY_STRING) {

        // Create a new file parser, give it a reference to the input file for all future operations.
        FileParser *fileData = new FileParser(inputData);

        // Based on the percent given, the estimated image size data will be the total number of bytes required to fit
        // the all the data in the image with the given percentage of pixels overwritten.
        float fileSize = fileData->estimateImageSizeData(percent);

        // Create a manipulator class, and pass a reference to the input image.
        ImageManipulator *manipulator = new ImageManipulator(inputImage);

        // This will scale the image, so that it will be capable of holding the total file size required for both the hidden file, and the
        // image data, so that the hidden file only takes up the given proportion of total data.
        manipulator->scaleImageForData(fileSize);

        // If noise was asked to be added, add it now.
        if (addNoise) {
            manipulator->addNoise(noise, noiseType);
        }
        // Using the given image, and the provided seed, shuffle the array pixel indices in a reproducible but random manner.
        fileData->generateRandomPixelArray(manipulator->img, seed);

        // Now that a random pixel array has been created, randomly insert bytes of data from the hidden file into random R, G, or B, elements
        // of pixels.
        manipulator->seedImage(fileData->randomPixelArray, fileData->dataFileVector, outputFile);
        return 0;
    }

    //With only input and output, assume extraction
     else if (inputImage != EMPTY_STRING && outputFile != EMPTY_STRING) {

        cout << "EXTRACTING FILE " << endl;

        // Create a new image manipulator using the input image.
        ImageManipulator *extractor = new ImageManipulator(inputImage);

        // Using the given image, extract the output file, using the given seed.
        extractor->extractSeededImage(outputFile, seed);

        return 0;
    }
}
