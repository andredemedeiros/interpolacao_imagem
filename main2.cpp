#include <iostream>
#include <opencv2/opencv.hpp>

cv::Vec3b bilinearInterpolate(const cv::Mat& image, float x, float y) {
    int x1 = std::floor(x);
    int y1 = std::floor(y);
    int x2 = std::min(x1 + 1, image.cols - 1); 
    int y2 = std::min(y1 + 1, image.rows - 1); 

    float dx = x - x1;
    float dy = y - y1;

    cv::Vec3b p1 = image.at<cv::Vec3b>(y1, x1);
    cv::Vec3b p2 = image.at<cv::Vec3b>(y1, x2);
    cv::Vec3b p3 = image.at<cv::Vec3b>(y2, x1);
    cv::Vec3b p4 = image.at<cv::Vec3b>(y2, x2);

    cv::Vec3b result;
    for (int c = 0; c < 3; ++c) {
        float top = p1[c] * (1 - dx) + p2[c] * dx;
        float bottom = p3[c] * (1 - dx) + p4[c] * dx;
        result[c] = cv::saturate_cast<uchar>(top * (1 - dy) + bottom * dy);
    }
    return result;
}

int main() {
    std::string imagePath = "imgs/3.1.jpg";

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    if (image.empty()) {
        std::cerr << "Error" << std::endl;
        return -1;
    }

    int currentDpi = 72;

    int desiredDpi;
    std::cout << "DPI desejado:";
    std::cin >> desiredDpi;

    float scalingFactor = (float)desiredDpi / currentDpi;

    int newWidth = (int)(image.cols * scalingFactor);
    int newHeight = (int)(image.rows * scalingFactor);

    cv::Mat resizedImage(newHeight, newWidth, image.type());

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            float srcX = x / scalingFactor;
            float srcY = y / scalingFactor;

            resizedImage.at<cv::Vec3b>(y, x) = bilinearInterpolate(image, srcX, srcY);
        }
    }

    cv::namedWindow("DPI original", cv::WINDOW_NORMAL);
    cv::imshow("DPI original", image);

    cv::namedWindow("DPI reduzido", cv::WINDOW_NORMAL);
    cv::imshow("DPI reduzido", resizedImage);

    std::cout << "Pressione qualquer tecla" << std::endl;
    cv::waitKey(0);

    float inverseScalingFactor = 1.0f / scalingFactor;

    int enlargedWidth = (int)(resizedImage.cols * inverseScalingFactor);
    int enlargedHeight = (int)(resizedImage.rows * inverseScalingFactor);

    cv::Mat enlargedImage(enlargedHeight, enlargedWidth, resizedImage.type());

    for (int y = 0; y < enlargedHeight; ++y) {
        for (int x = 0; x < enlargedWidth; ++x) {
            float srcX = x / inverseScalingFactor;
            float srcY = y / inverseScalingFactor;

            enlargedImage.at<cv::Vec3b>(y, x) = bilinearInterpolate(resizedImage, srcX, srcY);
        }
    }

    cv::namedWindow("DPI interpolado", cv::WINDOW_NORMAL);
    cv::imshow("DPI interpolado", enlargedImage);

    cv::waitKey(0);

    cv::imwrite("imgs/3.2.tif", resizedImage);
    cv::imwrite("imgs/3.3.tif", enlargedImage);

    return 0;
}
