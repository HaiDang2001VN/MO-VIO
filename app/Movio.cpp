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

using namespace cv;
using namespace cv::xfeatures2d;
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
  cout << "Total " << filenames.size() << " images found.\n";

  // Creating a surf object
  Ptr<xfeatures2d::SURF> surf = xfeatures2d::SURF::create(400);

  for (size_t i = 0; i < filenames.size() - 1; ++i) {
//    cout << "Current Image: " << filenames[i] << "\t Next image: "
//         << filenames[i + 1] << endl;

    Mat currentFrame = imread(filenames[i]);
    Mat nextFrame = imread(filenames[i + 1]);

    if (!currentFrame.data || !nextFrame.data) {
      cout << " Error reading images " << endl;
      return false;
    }

    vector<Point2f> currentFrameKeypoints, nextFrameKeypoints;

    // Detect surf features for the current frame
    Movio::getSURFFeatures(currentFrame, surf, currentFrameKeypoints);
//    Movio::getFastFeatures(currentFrame, currentFrameKeypoints);

    // Track the same features in the next frame
    Movio::trackFeatures(currentFrame, nextFrame, currentFrameKeypoints,
                         nextFrameKeypoints);

    vector<KeyPoint> kp;
    KeyPoint::convert(nextFrameKeypoints, kp);
    drawKeypoints(nextFrame, kp, nextFrame);



    if (!currentFrame.data)
      cerr << "Problem loading image!!!" << endl;

    imshow("Image", nextFrame);

    if (waitKey(1) >= 0)
      break;
  }

  return true;
}

void Movio::getSURFFeatures(const Mat& src, Ptr<xfeatures2d::SURF> detector,
                            vector<Point2f>& keypoints) {
  vector<KeyPoint> keyPoint;
  detector->detect(src, keyPoint);
  KeyPoint::convert(keyPoint, keypoints);
  cout << "\tSURF features: Found " << keypoints.size()
       << " key-points in the image." << endl;
}

void Movio::getFastFeatures(const Mat& src, vector<Point2f>& keypoints) {
  vector<KeyPoint> keyPoint;
  FAST(src, keyPoint, 20, true);
  KeyPoint::convert(keyPoint, keypoints, vector<int>());
  cout << "\tFast features: Found " << keypoints.size()
       << " key-points in the image." << endl;
}

void Movio::trackFeatures(const Mat& src1, const Mat& src2,
                          vector<Point2f>& point1, vector<Point2f>& point2) {
  vector<uchar> status;
  vector<float> error;
  Size windowSize = Size(25, 25);

  // Track features using KLT
  calcOpticalFlowPyrLK(src1, src2, point1, point2, status, error, windowSize,
                       3);

  if (point1.size() != point2.size()) {
    cout
        << "Track features: Features tracked does not have same dimension. Point1: "
        << point1.size() << "Point2: " << point2.size() << endl;
    return;
  }
  // Remove out points where tracking failed
  int falseCount = 0;
  for (size_t i = 0; i < point2.size(); i++) {
    Point2f pt = point2.at(i - falseCount);
    if ((status.at(i) == 0) || (pt.x < 0) || (pt.y < 0)) {
      point1.erase(point1.begin() + (i - falseCount));
      point2.erase(point2.begin() + (i - falseCount));
      falseCount++;
    }
  }
}

