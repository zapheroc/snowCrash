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

#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <algorithm> // for std::shuffle
#include <fstream> // for input and output streams
#include <vector> // for vectors containing data
#include <cmath> // for sqrt()
#include <iostream>
#include <random>

#include "CImg.h"
#include "pcg_random.hpp"

class FileParser {
public:
	FileParser(std::string fileName);
	FileParser();
	virtual ~FileParser();

	// Estimate how much space the data will take up, so that it is possible to accurate predict the required
	// image size given the input data ratio.
	float estimateImageSizeData(float ratioData);

	// To place a data file into an image, an array of pixel indices is created, and shuffled using a PRNG.
	// Bytes are then inserted at the indices of the shuffled array.
	void generateRandomPixelArray(const cimg_library::CImg<unsigned char> &img, std::string seedString);

	// To properly know the length of the file to extract, the file size must be embedded at the beginning of the snowCrash.
	// This function takes an unsigned long which represents the file size, and converts it to bytes for writing to the snowCrash.
	// Essentially, converts an unsigned long to an array of bytes.
	void longToBytes(unsigned long n, unsigned char ar[sizeof(unsigned long)]);
	// This does the opposite of longToBytes. It reads the first sizeof(unsigned long) bytes from a snowCrash and converts it
	// to and unsigned long so that the program knows how many bytes to extract.
	// Essentially, converts and array of bytes to an unsigned long.
	unsigned long bytesToLong(unsigned char n[sizeof(unsigned long)]);

	// Calculate proper width for a simple data to image conversion, straight from file.
	void calculateImageWidthHeightFromFile(unsigned int &imageWidth, unsigned int &imageHeight);

	const int LONG_LENGTH = sizeof(unsigned long);

	// The data byte array as read from a file.
	std::vector<char> dataFileVector;

	// This array of pixel indices is shuffled, and bytes of data are inserted in the image at the indices of the shuffled array.
	std::vector<unsigned long> randomPixelArray;

	// The size of the total data file.
	long fileSize = 0;
};

#endif // FILEPARSER_H
