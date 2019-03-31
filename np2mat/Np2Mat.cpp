//
// Created by celine on 27/03/19.
//

#include "Np2Mat.h"

// convert numpy array to vector<Mat> or vector<Mat_<float>> channels
void npch2ch(ndarray& ch1, ndarray& ch2, ndarray& ch3){
    int row = ch1.shape(0); int col = ch1.shape(1);
    float* ch1_pt = reinterpret_cast<float*>(ch1.get_data());
    float* ch2_pt = reinterpret_cast<float*>(ch2.get_data());
    float* ch3_pt = reinterpret_cast<float*>(ch3.get_data());
    Mat ch1_mat(row, col, CV_32FC1, ch1_pt);
    Mat ch2_mat(row, col, CV_32FC1, ch2_pt);
    Mat ch3_mat(row, col, CV_32FC1, ch3_pt);

    vector<Mat> channels = {ch1_mat, ch2_mat, ch3_mat};
    Mat testimage; merge(channels, testimage);
    testimage.convertTo(testimage, CV_8UC3);
    imshow("test", testimage);
    waitKey();
//    return channels;
}

// convert numpy array to vector<Mat> but this time assuming one ndarray which is contiguous
// and assume the array is channel FIRST
// ndarray -> vector<Mat>, vector<Mat_<>>
void np2ch(ndarray& image){
    int row = image.shape(1); int col = image.shape(2); int depth = image.shape(0);
    float* image_pt = reinterpret_cast<float*>(image.get_data());
    Mat image_mat(row, col, CV_32FC3, image_pt);
    vector<Mat> channels(3);
    split(image_mat, channels);
    Mat testimage; merge(channels, testimage);
    testimage.convertTo(testimage, CV_8UC3);
    imshow("test", testimage);
    waitKey();
//    return channels;
}

// convert numpy array to Mat
void np2mat(ndarray& image){
    int row = image.shape(0); int col = image.shape(1); int depth = image.shape(2);

    // get raw pointer of image and use it to construct mat
    float* image_pt = reinterpret_cast<float*>(image.get_data());

    // get them as a whole
    int size[3] = {row, col, depth};
    Mat_<float> image_mat(3, size, image_pt);
//    return image_mat;
}

void mat2np(vector<Mat_<float>>& img_mat, Mat_<float>& img_np, int row, int col, int depth, int gap){
    for (int k = 0; k < depth; ++k){
        Mat_<float> curr = img_mat[k];
        for(int i = 0; i < row; ++i){
            for(int j = 0; j < col; ++j){
                img_np(i,j,k) = curr(i+gap,j+gap);
            }
        }
    }
}

/* boost python wrapper */
BOOST_PYTHON_MODULE(np2mat)
{
    Py_Initialize();
    boost::python::numpy::initialize();
    def("npch2ch", npch2ch);
    def("np2ch", np2ch);
    def("np2mat", np2mat);
    def("mat2np", mat2np);
}
