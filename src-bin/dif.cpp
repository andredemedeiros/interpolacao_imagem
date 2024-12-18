#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    // Carrega a primeira imagem (imagem original) com leitura em cor
    cv::Mat img1 = cv::imread("../imgs/Fig0220(a)(chronometer 3692x2812 2pt25 inch 1250 dpi).tif", cv::IMREAD_COLOR);

    // Carrega a segunda imagem (imagem redimensionada com interpolação bilinear)
    cv::Mat img2 = cv::imread("../imgs/resized_image_bilinear.tif", cv::IMREAD_COLOR);  

    // Verifica se as imagens foram carregadas corretamente
    if (img1.empty() || img2.empty()) {
        std::cerr << "Erro ao carregar as imagens" << std::endl;  // Se não foi possível carregar as imagens, exibe um erro
        return -1;
    }

    // Exibe informações sobre as imagens: tamanho (dimensões) e tipo (tipo de dados dos pixels)
    std::cout << "Imagem 1 - Tamanho: " << img1.size() << ", Tipo: " << img1.type() << std::endl;
    std::cout << "Imagem 2 - Tamanho: " << img2.size() << ", Tipo: " << img2.type() << std::endl;

    // Verifica se as imagens têm o mesmo tamanho
    if (img1.size() != img2.size()) {
        // Se os tamanhos forem diferentes, redimensiona a imagem 2 para ter o mesmo tamanho da imagem 1
        std::cout << "Redimensionando a imagem 2 para o tamanho da imagem 1" << std::endl;
        cv::resize(img2, img2, img1.size());  // Redimensiona img2 para o tamanho de img1
    }

    // Verifica se as imagens têm o mesmo número de canais (por exemplo, RGB vs escala de cinza)
    if (img1.channels() != img2.channels()) {
        // Se o número de canais for diferente, converte a imagem com menos canais para o mesmo número de canais da outra
        std::cout << "Convertendo para o mesmo número de canais" << std::endl;
        if (img1.channels() == 3) {
            // Se img1 tem 3 canais (cor), converte img2 para escala de cinza
            cv::cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
        } else {
            // Caso contrário, converte img1 para escala de cinza
            cv::cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
        }
    }

    // Cria uma imagem para armazenar a diferença absoluta entre as duas imagens
    cv::Mat diff;
    cv::absdiff(img1, img2, diff);  // Calcula a diferença entre img1 e img2 e armazena em 'diff'

    // Salva a imagem de diferença em um arquivo
    bool sucesso = cv::imwrite("../imgs/diferenca.tif", diff);
    if (!sucesso) {
        // Se não for possível salvar a imagem de diferença, exibe um erro
        std::cerr << "Erro ao salvar a imagem de diferença" << std::endl;
        return -1;
    }

    // Exibe a imagem de diferença na tela
    cv::imshow("Imagem de diferenca", diff);
    cv::waitKey(0);  // Aguarda o usuário pressionar uma tecla para fechar a janela

    return 0;  // Finaliza o programa
}
