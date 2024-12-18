#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Carregar as imagens
    cv::Mat img1 = cv::imread("imgs/3.1.png");
    cv::Mat img2 = cv::imread("imgs/3.3.png");

    // Verificar se as imagens foram carregadas corretamente
    if (img1.empty() || img2.empty()) {
        std::cerr << "Erro ao carregar as imagens!" << std::endl;
        return -1;
    }

    // Imprimir as informações das imagens
    std::cout << "Imagem 1 - Tamanho: " << img1.size() << ", Tipo: " << img1.type() << std::endl;
    std::cout << "Imagem 2 - Tamanho: " << img2.size() << ", Tipo: " << img2.type() << std::endl;

    // Verificar se as imagens têm o mesmo tamanho e tipo
    if (img1.size() != img2.size()) {
        std::cout << "Redimensionando a imagem 2 para o tamanho da imagem 1..." << std::endl;
        cv::resize(img2, img2, img1.size());
    }

    // Converter as imagens para o mesmo tipo (exemplo: escala de cinza)
    if (img1.channels() != img2.channels()) {
        std::cout << "Convertendo para o mesmo número de canais..." << std::endl;
        if (img1.channels() == 3) {
            cv::cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
        } else {
            cv::cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
        }
    }

    // Subtrair as imagens
    cv::Mat diff;
    cv::absdiff(img1, img2, diff);  // absdiff calcula a diferença absoluta pixel a pixel

    // Salvar a imagem de diferença
    bool sucesso = cv::imwrite("imgs/dif3.2.png", diff);
    if (!sucesso) {
        std::cerr << "Erro ao salvar a imagem de diferença!" << std::endl;
        return -1;
    }

    // Exibir a imagem de diferença
    cv::imshow("Imagem de Diferenca", diff);
    cv::waitKey(0);  // Espera uma tecla ser pressionada

    return 0;
}
