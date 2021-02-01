#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

int main(int argc, const char **argv)
{
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{face_cascade|K:\\msys64\\mingw64\\share\\opencv4\\haarcascades\\haarcascade_frontalface_alt.xml|Path to face3 cascade.}"
                             "{eyes_cascade|K:\\msys64\\mingw64\\share\\opencv4\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml|Path to eyes cascade.}"
                             "{camera|0|Camera device number.}");
    parser.about("\nThis program demonstrates using the cv::CascadeClassifier class to detect objects(Face + eyes) in a video stream.\n"
                 "You can use Haar or LBP features.\n\n");
    parser.printMessage();

    string face_cascade_name = samples::findFile(parser.get<string>("face_cascade"));
    string eyes_cascade_name = samples::findFile(parser.get<string>("eyes_cascade"));

    if (!face_cascade.load(face_cascade_name))
    {
        cout << "--(!)Error loading face cascade\n"
             << endl;
        return -1;
    }
    if (!eyes_cascade.load(eyes_cascade_name))
    {
        cout << "--(!)Error loading eyes cascade\n"
             << endl;
        return -1;
    }

    Mat img = imread("1.jpg");
    if (!img.empty())
    {
        namedWindow("Á³", 0);
        imshow("Á³", img);
        detectAndDisplay(img);
        waitKey(0);
    }

    cout << cv::getVersionMajor() << cv::getVersionMinor() << cv::getVersionRevision() << endl;
    return 0;
}

