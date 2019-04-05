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

#include "ImageManipulator.h"

using namespace std;

//For pure extraction, and image is not needed as a parameter.
ImageManipulator::ImageManipulator() {
}

ImageManipulator::ImageManipulator(cimg_library::CImg<unsigned char> &dummyImage) {
	img = dummyImage;
}

ImageManipulator::ImageManipulator(string imageName) {
	img.load(imageName.c_str());
}

void ImageManipulator::scaleImageForData(float fileSize) {

	using namespace cimg_library;

	if ((img.width() * img.height()) <= fileSize) {

		unsigned int finalHeight = ceil(sqrt((fileSize * img.height()) / img.width()));
		unsigned int finalWidth = ceil(sqrt((fileSize * img.width()) / img.height()));

		cout << "Original Width: " << img.width() << " Original Height: " << img.height() << endl;
		cout << "Final Width: " << finalWidth << " Final Height: " << finalHeight << endl;
		img.resize(finalWidth, finalHeight);

	} else {
		cout << "Image did not need resizing: w*h=" << img.width() * img.height() << endl;
	}
}

void ImageManipulator::seedImage(vector<unsigned long> &randomPixelArray,
		vector<char> &dataFileVector, string outputFile) {

	unsigned char *pixel = img.data();
	cout << "Scaled image size: " << img.size() << " File to embed size: " << dataFileVector.size()
			<< endl;
	for (unsigned int i = 0; i < dataFileVector.size(); i++) {
		*(pixel + randomPixelArray.at(i)) = dataFileVector.at(i);
	}

	cout << "Seeding loop finished. Saving file as png." << endl;
	img.save_png(outputFile.c_str());
}

void ImageManipulator::extractSeededImage(string outputFile, string seedString) {

	ofstream outData(outputFile, ios::binary);

	unsigned char *pixel = img.data();

	FileParser *fileExtractor = new FileParser;

	fileExtractor->generateRandomPixelArray(img, seedString);

	unsigned char* fileSizeAsArrayOfBytes = new unsigned char[fileExtractor->LONG_LENGTH];

	for (int i = 0; i < fileExtractor->LONG_LENGTH; i++) {
		fileSizeAsArrayOfBytes[i] = *(pixel + (fileExtractor->randomPixelArray.at(i)));
	}

	unsigned long fileLength = fileExtractor->bytesToLong(fileSizeAsArrayOfBytes);
	cout << "File Length as extracted: " << fileLength << endl;

	for (unsigned long pix = 0; pix < fileLength; pix++) {
		outData
				<< *(pixel + (fileExtractor->randomPixelArray.at(pix + fileExtractor->LONG_LENGTH)));
	}
	delete[] fileSizeAsArrayOfBytes;
}

void ImageManipulator::addNoise(double sigma, unsigned int type) {
	cout << "Adding noise." << endl;
	// Prevent out of bounds input.
	if (type > 4) {
		type = 4;
	}
	// Use CImg library to generate noise for us.
	img.noise(sigma, type);
}

ImageManipulator::~ImageManipulator() {

}
