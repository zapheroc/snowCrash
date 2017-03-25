#include "FileParser.h"
#include "CImg.h"

using namespace std;


FileParser::FileParser()
{
    return;
}

FileParser::FileParser(string fileName )
{
   ifstream inData(fileName, ios::binary|ios::ate);

    if (!inData) {
        cerr << "Input File was not found.";
        exit(1);
    }

    ifstream::pos_type pos = inData.tellg();
    dataFileVector.resize( static_cast<long>(pos) + LONG_LENGTH );
    inData.seekg(0, ios::beg);
    inData.read(dataFileVector.data() + LONG_LENGTH, pos);
    inData.close();

    fileSize = dataFileVector.size();

    cout << "file size is: " << fileSize - LONG_LENGTH << endl;
    unsigned char fileSizeAsArrayOfBytes[LONG_LENGTH];
    longToBytes(fileSize - LONG_LENGTH, fileSizeAsArrayOfBytes);
    for (int i = 0; i < LONG_LENGTH; i++) {
        dataFileVector.at(i) = fileSizeAsArrayOfBytes[i];
        //cout << "The byte written to the start was: " << static_cast<int>(fileSizeAsArrayOfBytes[i]) << endl;
    }
    //cout << "First element of actual file is: " << dataFileVector.at(8) << endl;

}

float FileParser::estimateImageSizeData( float ratioData = 1.0) {

    if (ratioData > 1 || ratioData < 0) {
        cerr << "The ratio was out of range. Use decimal values between 1 and 0" << endl;
        exit(1);
    }
    return (fileSize / ratioData) / 3;
}

void FileParser::generateRandomPixelArray(const cimg_library::CImg<unsigned char> &img, long seed ) {
    using namespace cimg_library;

    unsigned long imgSize = img.size() - 1;
    cout << "The image size for random generation was: " << imgSize << endl;
    randomPixelArray.resize(imgSize);

    for (unsigned long i = 0; i < imgSize; i++) {
        randomPixelArray[i] = i;
    }

    //boost::random::mt19937 engine(seed);

    //boost::variate_generator<boost::mt19937&, boost::uniform_int<> > random_number_shuffler(engine, boost::uniform_int<>());
    //std::random_shuffle(randomPixelArray.begin(), randomPixelArray.end(), random_number_shuffler);

    mt19937 engine(seed);

    std::shuffle(randomPixelArray.begin(), randomPixelArray.end(), engine);

    //THIS IS THE PROBLEM, without if statement, it clipped to zero on extraction
    if (dataFileVector.size() > 0) {
        randomPixelArray.resize(dataFileVector.size());
    }
    cout << "Random Pixel Array Size: " << randomPixelArray.size() << endl;
}

void FileParser::longToBytes(unsigned long n, unsigned char ar[sizeof(unsigned long)]) {
    for (int i = 0; i < LONG_LENGTH; i++) {
        ar[i] = (n >> i*8) & 0xFF;
    }
}

unsigned long FileParser::bytesToLong(unsigned char n[sizeof(unsigned long)]) {
    unsigned long r = 0;
    for (int i = 0; i < LONG_LENGTH; i++) {
      //  cout << "pos " << i << endl;
      //  cout << "array value: " << static_cast<int>(n[i]) << endl;
        r = (r | n[(LONG_LENGTH-1)-i]);
      //  cout << "r value: " << r << endl;
        if (i != LONG_LENGTH - 1) {
            r = (r << 8);
        }
    }
    return r;
}

FileParser::~FileParser()
{
    //dtor
}
