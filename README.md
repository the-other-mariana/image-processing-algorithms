# Image Processing Algorithms in C++

This repo contains a handful of the most famous algorithms for image processing, coded from scratch just using math and several data structures such as vectors and queues. <br />

## Specifications

You will need to install OpenCV for C++. <br />

## Usage

I recommend using Microsoft Visual Studio for the debugging and running of the scripts. Once you have downloaded OpenCV for C++, open Visual Studio and follow the steps: <br />

1. Create a New Empty Project. <br />

2. Go to the `Solution Explorer` and over the `Source Files` folder, and `right click > Add > C++ File`. <br />

3. Paste in `Source.cpp` file the script you choose. <br />

4. Go to `Project > Project Properties`. <br />

![img](https://github.com/the-other-mariana/image-processing-algorithms/blob/master/resources/properties.png?raw=true) <br />

5. Click on `VC++` option and change the following directories in **bold**. <br />

![img](https://github.com/the-other-mariana/image-processing-algorithms/blob/master/resources/vc-directories.png?raw=true) <br />

6. Go to this directory in your `C:\` disk and copy the file name. <br />

![img](https://github.com/the-other-mariana/image-processing-algorithms/blob/master/resources/library-name.png?raw=true) <br />

7. Then, click on `Linker > Input` where you will paste the copied name from last step. <br />

![img](https://github.com/the-other-mariana/image-processing-algorithms/blob/master/resources/paste-name.png?raw=true) <br />

8. Click on Apply. <br />

9. Change the top `Debug` option to `x64`. <br />

10. Run the code. <br />

## Content

### 001 [Image Thresholding](https://github.com/the-other-mariana/image-processing-algorithms/tree/master/001-threshold)

![image](https://github.com/the-other-mariana/image-processing-algorithms/blob/master/001-threshold/output.png?raw=true) <br/>

### 002 [Image Mask](https://github.com/the-other-mariana/image-processing-algorithms/tree/master/002-mask)

![image](https://github.com/the-other-mariana/image-processing-algorithms/blob/master/002-mask/result.png?raw=true) <br/>

### 005 [Image Equalization & Histogram](https://github.com/the-other-mariana/image-processing-algorithms/tree/master/005-equalization)

#### Equalization 

![image](https://github.com/the-other-mariana/image-processing-algorithms/blob/master/005-equalization/equalized.png?raw=true) <br/>

#### Resulting Histogram

![image](https://github.com/the-other-mariana/image-processing-algorithms/blob/master/005-equalization/equalized-histogram.png?raw=true) <br/>