//
//  main.cpp
//  rover-simulator
//
//  Created by Ryu Dae-ha on 5/17/26.
//

#include <iostream>
#include "Matrix2D.hpp"
#include <opencv2/opencv.hpp>
#include <algorithm>

int main(int argc, const char * argv[]) {
    Matrix2D mat(10, 10, 0);
    mat.at(0, 0) = 1;
    mat.at(1, 1) = 2;
    mat.at(2, 2) = 3;
    mat.at(9, 9) = 4;

    std::cout << "Matrix (" << mat.rows() << "x" << mat.cols() << ")" << '\n';
    for (std::size_t r = 0; r < mat.rows(); ++r) {
        for (std::size_t c = 0; c < mat.cols(); ++c) {
            std::cout << mat.at(r, c) << (c + 1 == mat.cols() ? '\n' : ' ');
        }
    }

    // Visualize the matrix with OpenCV
    // Normalize values to 0-255 for display
    int minVal = mat.at(0,0), maxVal = mat.at(0,0);
    for (std::size_t r = 0; r < mat.rows(); ++r) {
        for (std::size_t c = 0; c < mat.cols(); ++c) {
            minVal = std::min(minVal, mat.at(r, c));
            maxVal = std::max(maxVal, mat.at(r, c));
        }
    }
    int range = std::max(1, maxVal - minVal);

    cv::Mat gray(static_cast<int>(mat.rows()), static_cast<int>(mat.cols()), CV_8UC1);
    for (int r = 0; r < gray.rows; ++r) {
        for (int c = 0; c < gray.cols; ++c) {
            int v = mat.at(static_cast<std::size_t>(r), static_cast<std::size_t>(c));
            int norm = (v - minVal) * 255 / range; // 0..255
            gray.at<unsigned char>(r, c) = static_cast<unsigned char>(norm);
        }
    }

    // Apply a colormap for better visualization
    cv::Mat color;
    cv::applyColorMap(gray, color, cv::COLORMAP_TURBO);

    cv::resize(color, color, cv::Size(color.cols * 50, color.rows * 50), 0, 0, cv::INTER_NEAREST);
    cv::imshow("Map", color);
    cv::waitKey(0);

    return EXIT_SUCCESS;
}
