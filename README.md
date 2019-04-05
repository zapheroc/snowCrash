# **snowCrash**
SnowCrash is a data to png converter. It can turn any file into a png, storing the data in the colors of the pixels. It also supports embedding data in existing images. You can stick a text file into a picture, for example. 

A small binary data to png image converter, which will hopefully grow into something more complex.

## **Installation Steps**

Clone the repository, and run the make script, either for linux or OSX. This should build snowCrash. There are some requirements, specifically libpng or imagemagic, and pthread are required. 

## *Security*

The order of the bytes of data in a file are randomized using and PCG random number generation engine. This means to extract an image, you will need to know the seed which was used to create it. That said, this is NOT a security program. Although in theory, it may be difficult to detect or extract images, this software is not intended to secure files. If you want to secure a file, encrypt it, and then use snowCrash on the encrypted file. 

That said, it would be very interesting to see how hard it would be to extract a file which has been hidden in a png, and seeded with noise.

## *Useage*

Once you compile snowCrash, it is a command line only tool.

To extract, provide only an input and output, along with a seed.
To simply convert data to an image, provide only a data file and an output, along with a seed.
Useage:  
-i to specify input file. 
-o to specify output file. 
-d to specify data file to place into the image. 
-s to specify seed, default is "password". This can be any string.                 
-p to specify image density, default is 1. This must be between 0-1. 
-n to specify amount of noise to autogenerate. Default is 0. 
-t specify the type of noise. 0=gaussian, 1=uniform, 2=Salt and Pepper (default), 3=Poisson or 4=Rician.

Example embedding file: snowCrash -d file.txt -i imageToHideIn.png -s coolpassword -o outputFile.png.
Example extracting file: snowCrash -i hidden.png -s coolpassword -o outputFile.originalExtension.
Example converting data straight to PNG: snowCrash -d file.txt -o outputFile.png
