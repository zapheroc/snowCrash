/*
 * A simple command line tool which allows data files to be converted to images, or hidden within other images.
 *
 *  Copyright 2017 Anon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <fstream>
#include <cstdlib> // for the exit function
#include <getopt.h>

#include "CImg.h"
#include "FileParser.h"
#include "ImageManipulator.h"

using namespace std;

const string VERSION = "2.0";
const string EMPTY_STRING = "//empty\\";

int main(int argc, char *argv[]) {

	int opt;
	string inputImage = EMPTY_STRING;
	string inputData = EMPTY_STRING;
	string outputFile = EMPTY_STRING;
	string extractFile = EMPTY_STRING;
	string seedString = "password";
	int noiseType = 2;
	//seed_seq seed(seedString.begin(), seedString.end());
	float percent = 1;
	double noise = 0;
	bool addNoise = false;

	string help =
			"To extract, provide only an input and output, along with a seed.\nTo simply convert data to an image, provide only a data file and an output, along with a seed.\nUseage:  \n-i to specify input file. \n-o to specify output file. \n-d to specify data file to place into the image. \n-s to specify seed, default is \"password\". This can be any string. \
                \n-p to specify image density, default is 1. This must be between 0-1. \n-n to specify amount of noise to autogenerate. Default is 0. \n-t specify the type of noise. 0=gaussian, 1=uniform, 2=Salt and Pepper (default), 3=Poisson or 4=Rician.\n\nExample: snowCrash -d file.txt -i imageToHideIn.png -s coolpassword -o outputFile.png.";

	if (argc == 1) {
		cout << help << endl;
		return 0;
	}

//	Based on the arguments that the users provides, store each of those arguments in its appropriate variable,
//	and then later on use those arguments for switching the behavior of the program. The use of each argument
//	is described in the help string.

	while ((opt = getopt(argc, argv, "d:i:n:o:s:t:p:vh")) != -1) {
		switch (opt) {
		case 's':
			seedString = optarg;
			//seed = seed_seq(seedString.begin(), seedString.end());
			break;
		case 'p':
			percent = atof(optarg);
			cout << percent << " ratio" << endl;
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
			break;
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
			cout << help << endl;
			return 0;
		default:
			cerr << "No required options specified. Please use -h to get help." << endl;
		}
	}

//	In order to prevent users from accidentally destroying or overwriting files, this quick check
//	forces the user to confirm file overwrite.
	ifstream does_exist(outputFile);
	if (does_exist) {
		string response = "";
		cout << "File exists. Overwrite? (y/n): ";
		cin >> response;
		if (response != "y" && response != "Y") {
			cerr << "Aborted " << endl;
			exit(-1);
		}
	}
	does_exist.close();

//	If you have an input, output, and data, then the user is attempting to place a data file into an already existing image.
	if (inputImage != EMPTY_STRING && outputFile != EMPTY_STRING && inputData != EMPTY_STRING) {

		cout << "Hiding data in given image." << endl;

		// The FileParser class handles all file oriented operations, including the shuffle of the pixel index array for seeding.
		FileParser *fileData = new FileParser(inputData);

		// Based on the percent given, the estimated image size data will be the total number of bytes required to fit
		// the all the data in the image with the given percentage of pixels overwritten.
		float fileSize = fileData->estimateImageSizeData(percent);

		// The image manipulator class handles loading images, as well as reading and writing to them.
		ImageManipulator *manipulator = new ImageManipulator(inputImage);

		// This will scale the image, so that it will be capable of holding the total file size required for both the hidden file, and the
		// image data, so that the hidden file only takes up the given percentage of total data.
		manipulator->scaleImageForData(fileSize);

		// Based on user specification, noise can be added into the image to disguise the file data.
		if (addNoise) {
			manipulator->addNoise(noise, noiseType);
		}

		// Using the given image, and the provided seed, shuffle the array pixel indices in a reproducible but random manner.
		fileData->generateRandomPixelArray(manipulator->img, seedString);

		// Now that a random pixel array has been created, randomly insert bytes of data from the hidden file into random R, G, or B, elements
		// of pixels.
		manipulator->seedImage(fileData->randomPixelArray, fileData->dataFileVector, outputFile);
		return 0;
	}

//	With only input and output, assume extraction
	else if (inputImage != EMPTY_STRING && outputFile != EMPTY_STRING) {

		cout << "Extracting file from input image." << endl;

		// Create in image manipulator which will load the image based on the given string and read from it appropriately.
		ImageManipulator *extractor = new ImageManipulator(inputImage);

		extractor->extractSeededImage(outputFile, seedString);

		return 0;
	}
//	With only input data and an output file, just compress the input data to a png.
	else if (inputData != EMPTY_STRING && outputFile != EMPTY_STRING) {
		cout << "Compressing data straight to png." << endl;

		FileParser *fileData = new FileParser(inputData);

		unsigned int imageWidth = 0;
		unsigned int imageHeight = 0;
		fileData->calculateImageWidthHeightFromFile(imageWidth, imageHeight);

		// Because we're not loading an image, we have to create a dummy image, and pass it to our image manipulator,
		// so that the image manipulator can write to it.
		cimg_library::CImg<unsigned char> img(imageWidth, imageHeight, 1, 3);

		ImageManipulator *manipulator = new ImageManipulator(img);

		fileData->generateRandomPixelArray(manipulator->img, seedString);

		// By acting as if the dummy image was a loaded image, we can use the same extraction command
		// to extract from all snowCrash images.
		manipulator->seedImage(fileData->randomPixelArray, fileData->dataFileVector, outputFile);

		return 0;
	} else {
		cout << "Incorrect arguments." << endl << help << endl;
	}

}
