/*
 * A simple command line tool which allows data files to be converted to images, or hidden within other images.
 *
 * Copyright 2017 Anon
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

#include "FileParser.h"
//#include "CImg.h"

using namespace std;

FileParser::FileParser() {
}

FileParser::FileParser(string fileName) {
	ifstream inData(fileName, ios::binary | ios::ate);

	if (!inData) {
		cerr << "Input File was not found.";
		exit(-1);
	}

	ifstream::pos_type pos = inData.tellg();
	dataFileVector.resize(static_cast<long>(pos) + LONG_LENGTH);
	inData.seekg(0, ios::beg);
	inData.read(dataFileVector.data() + LONG_LENGTH, pos);
	inData.close();

	fileSize = dataFileVector.size();

	cout << "file size is: " << fileSize - LONG_LENGTH << endl;
	unsigned char fileSizeAsArrayOfBytes[LONG_LENGTH];
	longToBytes(fileSize - LONG_LENGTH, fileSizeAsArrayOfBytes);
	for (int i = 0; i < LONG_LENGTH; i++) {
		dataFileVector.at(i) = fileSizeAsArrayOfBytes[i];
	}
}

float FileParser::estimateImageSizeData(float ratioData = 1.0) {

	if (ratioData > 1 || ratioData < 0) {
		cerr << "The ratio was out of range. Use decimal values between 1 and 0"
				<< endl;
		exit(-1);
	}
	return (fileSize / ratioData) / 3;
}
// TODO move this function to ImageManipulator, because it is image related.
void FileParser::generateRandomPixelIndicesArray(unsigned long imgSize, string seedString) {

	imgSize = imgSize - 1;
	//unsigned long imgSize = img.size() - 1;
	cout << "The image size for random generation is: " << imgSize << endl;
	randomPixelIndicesArray.resize(imgSize);

	for (unsigned long i = 0; i < imgSize; i++) {
		randomPixelIndicesArray[i] = i;
	}

	std::seed_seq seed(seedString.begin(), seedString.end());
	pcg64_c1024 engine(seed);

	// Outdated mt19937 engine was replaced with pcg library. Maybe more secure?
	//mt19937 engine(seed);

	pcg_extras::shuffle(randomPixelIndicesArray.begin(), randomPixelIndicesArray.end(),
			engine);

	// First shuffle every pixel possible. Then trim it down to the size of the data to put into the image,
	// so that randimPixelArray is a list of indices of pixels the same size as the dataFile.
	if (dataFileVector.size() > 0) {
		randomPixelIndicesArray.resize(dataFileVector.size());
	}
}


void FileParser::longToBytes(unsigned long n,
		unsigned char ar[sizeof(unsigned long)]) {
	for (int i = 0; i < LONG_LENGTH; i++) {
		ar[i] = (n >> i * 8) & 0xFF;
	}
}

unsigned long FileParser::bytesToLong(unsigned char* n) {
	unsigned long r = 0;
	for (int i = 0; i < LONG_LENGTH; i++) {
		r = (r | n[(LONG_LENGTH - 1) - i]);
		if (i != LONG_LENGTH - 1) {
			r = (r << 8);
		}
	}
	return r;
}

void FileParser::calculateImageWidthHeightFromFile(unsigned int &imageWidth,
		unsigned int &imageHeight) {
	unsigned long numberOfBytes = dataFileVector.size() / 3;

	imageWidth = round(sqrt(numberOfBytes));

	if (imageWidth < 1) {
		imageWidth = 1;
	}
	imageHeight = imageWidth;

	while (imageWidth * imageHeight < numberOfBytes) {
		imageHeight++;
	}
	return;
}

FileParser::~FileParser() {
}
