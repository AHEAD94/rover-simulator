//
//  main.cpp
//  rover-simulator
//
//  Created by Ryu Dae-ha on 5/17/26.
//

#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "Matrix2D.hpp"
#include "Rover.hpp"

int main(int argc, const char * argv[]) {
    Matrix2D mat({
        {0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 0, 0, 1, 0},
        {1, 0, 2, 0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0, 2, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 9},
    });
    
    Rover rov(0, 0);

    std::cout << "Matrix (" << mat.rows() << "x" << mat.cols() << ")" << '\n';
    for (std::size_t r = 0; r < mat.rows(); ++r) {
        for (std::size_t c = 0; c < mat.cols(); ++c) {
            std::cout << mat.at(r, c) << (c + 1 == mat.cols() ? '\n' : ' ');
        }
    }
    
    std::cout << "Rover (" << rov.position().row << ", " << rov.position().col << ")" << std::endl;

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

    // Overlay rover position on the visualization
    {
        // Determine scale used in resize (assumes uniform integer scale)
        const int scale = 50; // must match the resize factor above

        // Fetch rover grid coordinates (row, col)
        auto pos = rov.position();

        // Convert grid coordinates to pixel coordinates in the resized image
        // x corresponds to column, y corresponds to row
        int x = static_cast<int>(pos.col) * scale + scale / 2;
        int y = static_cast<int>(pos.row) * scale + scale / 2;

        // Clamp to image bounds just in case
        x = std::max(0, std::min(x, color.cols - 1));
        y = std::max(0, std::min(y, color.rows - 1));

        // Draw a visible rectangle marker for the rover (centered on the tile)
        int half = std::max(6, scale / 2 - 10);
        cv::Point topLeft(x - half, y - half);
        cv::Point bottomRight(x + half, y + half);
        cv::rectangle(color, topLeft, bottomRight, cv::Scalar(0, 0, 255), -1, cv::LINE_AA);

        // Optional: draw a label
        const std::string label = "R";
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
        cv::Point textOrg(x - textSize.width / 2, y - textSize.height / 2);
        cv::putText(color, label, textOrg, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    }

    cv::imshow("Map", color);
    cv::waitKey(0);

    return EXIT_SUCCESS;
}

