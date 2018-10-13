/***************************************************************************
 MIT License

 Copyright (c) 2018 Rohit

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 *****************************************************************************/

#include <iostream>
#include <string>
#include <Movio.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;

Movio::Movio(string filepath, bool debug) {
  this->filepath = filepath;
  this->debug = debug;
  cout << " **** Monocular Visual Odometry initialized **** \nFilepath: "
       << this->filepath
       << "\nDebug: " << boolalpha << debug << endl;
}

Movio::~Movio() {
}

bool Movio::startOdom() {
  vector<String> filenames;
  namedWindow("Image", 1);

  glob(filepath, filenames);
  cout << "Total " << filenames.size() << " images found\n.";

  for (size_t i = 0; i < filenames.size(); ++i) {
    cout << "Image: " << filenames[i] << endl;
    Mat src = imread(filenames[i]);

    if (!src.data)
      cerr << "Problem loading image!!!" << endl;

    imshow("Image", src);

    if (waitKey(30) >= 0)
      break;
  }

  return true;
}
