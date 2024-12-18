#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    cv::Mat img1 = cv::imread("imgs/Fig0220(a)(chronometer 3692x2812 2pt25 inch 1250 dpi).tif", cv::IMREAD_COLOR);  // Abrir com leitura em cor
    cv::Mat img2 = cv::imread("imgs/resized_image_bilinear.tif", cv::IMREAD_COLOR);  

    if (img1.empty() || img2.empty()) {
        std::cerr << "Erro ao carregar as imagens" << std::endl;
        return -1;
    }

    std::cout << "Imagem 1 - Tamanho: " << img1.size() << ", Tipo: " << img1.type() << std::endl;
    std::cout << "Imagem 2 - Tamanho: " << img2.size() << ", Tipo: " << img2.type() << std::endl;

    if (img1.size() != img2.size()) {
        std::cout << "Redimensionando a imagem 2 para o tamanho da imagem 1" << std::endl;
        cv::resize(img2, img2, img1.size());
    }

    if (img1.channels() != img2.channels()) {
        std::cout << "Convertendo para o mesmo número de canais" << std::endl;
        if (img1.channels() == 3) {
            cv::cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
        } else {
            cv::cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
        }
    }

    cv::Mat diff;
    cv::absdiff(img1, img2, diff);  

    bool sucesso = cv::imwrite("imgs/diferenca.tif", diff);
    if (!sucesso) {
        std::cerr << "Erro ao salvar a imagem de diferença" << std::endl;
        return -1;
    }

    cv::imshow("Imagem de diferenca", diff);
    cv::waitKey(0);

    return 0;
}
