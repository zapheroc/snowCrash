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

#ifndef IMAGEMANIPULATOR_H
#define IMAGEMANIPULATOR_H

#include "CImg.h"
#include "FileParser.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>
//#include <random>

class ImageManipulator {
public:
	ImageManipulator();
	ImageManipulator(std::string imageName);
	ImageManipulator(cimg_library::CImg<unsigned char> &dummyImage);

	virtual ~ImageManipulator();

	// To extract the seeded image, a FileParser will be created, and used to shuffle the pixel array. Then, the next pixel
	// indices will be read from the image until the original filesize is reached.
	void extractSeededImage(std::string outputFile, std::string seedString);

	// Scale an input image to be the correct size to fit the given data.
	void scaleImageForData(float fileSize);

	// Using the random array of pixel indices, write at that location in the image file until all of the data file has been written.
	void seedImage(std::vector<unsigned long> &randomPixelArray, std::vector<char> &dataFileVector,
			std::string outputFile);

	// Add random noise to the image to disguise the image data.
	void addNoise(double sigma, unsigned int type);

	// The image object that will be manipulated by this image manipulator.
	cimg_library::CImg<unsigned char> img;
};

#endif // IMAGEMANIPULATOR_H
