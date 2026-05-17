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
#include "RoverController.hpp"

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
    RoverController controller(mat, rov);

    std::cout << "Matrix (" << mat.rows() << "x" << mat.cols() << ")" << '\n';
    for (std::size_t r = 0; r < mat.rows(); ++r) {
        for (std::size_t c = 0; c < mat.cols(); ++c) {
            std::cout << mat.at(r, c) << (c + 1 == mat.cols() ? '\n' : ' ');
        }
    }
    
    std::cout << "Rover (" << rov.position().row << ", " << rov.position().col << ")" << std::endl;

    auto render = [&](cv::Mat &out) {
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

        // Resize for visibility
        const int scale = 50;
        cv::resize(color, out, cv::Size(color.cols * scale, color.rows * scale), 0, 0, cv::INTER_NEAREST);

        // Overlay rover position on the visualization
        {
            auto pos = rov.position();
            int x = static_cast<int>(pos.col) * scale + scale / 2;
            int y = static_cast<int>(pos.row) * scale + scale / 2;
            x = std::max(0, std::min(x, out.cols - 1));
            y = std::max(0, std::min(y, out.rows - 1));
            int half = std::max(6, scale / 2 - 10);
            cv::Point topLeft(x - half, y - half);
            cv::Point bottomRight(x + half, y + half);
            cv::rectangle(out, topLeft, bottomRight, cv::Scalar(0, 0, 255), -1, cv::LINE_AA);
            const std::string label = "R";
            int baseline = 0;
            cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
            cv::Point textOrg(x - textSize.width / 2, y - textSize.height / 2);
            cv::putText(out, label, textOrg, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        }
    };

    cv::Mat vis;
    render(vis);
    cv::imshow("Map", vis);

    // Interactive loop: use arrow keys to move the rover; press 'q' or ESC to quit
    while (true) {
        int key = cv::waitKey(0);
        if (key == 27 || key == 'q' || key == 'Q' || key == 66) { // ESC or q to quit
            break;
        }

        bool moved = false;
        switch (key) {
            case 0x260000: // up
            case 82:       // up
            case 65362:    // up (X11)
            case 0:
                moved = controller.tryMove(Direction::North);
                break;
            case 0x280000: // down
            case 84:       // down
            case 65364:    // down (X11)
            case 1:
                moved = controller.tryMove(Direction::South);
                break;
            case 0x250000: // left (some backends)
            case 81:       // left (common)
            case 65361:    // left (X11)
            case 2:
                moved = controller.tryMove(Direction::West);
                break;
            case 0x270000: // right
            case 83:       // right
            case 65363:    // right (X11)
            case 3:
                moved = controller.tryMove(Direction::East);
                break;
            default:
                break;
        }

        if (moved) {
            // Optionally print updated position
            std::cout << "Rover (" << rov.position().row << ", " << rov.position().col << ")" << std::endl;
            render(vis);
            cv::imshow("Map", vis);
        }
    }

    return EXIT_SUCCESS;
}
