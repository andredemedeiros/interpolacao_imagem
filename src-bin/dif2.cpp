#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Carrega a primeira imagem (img1) a partir do arquivo especificado
    cv::Mat img1 = cv::imread("../imgs/2.1.png");

    // Carrega a segunda imagem (img2) a partir do arquivo especificado
    cv::Mat img2 = cv::imread("../imgs/2.3.png");

    // Verifica se as imagens foram carregadas corretamente
    if (img1.empty() || img2.empty()) {
        std::cerr << "Erro ao carregar as imagens!" << std::endl;  // Se alguma imagem não for carregada, exibe erro
        return -1;
    }

    // Exibe informações sobre as imagens: tamanho (dimensões) e tipo (formato de pixel)
    std::cout << "Imagem 1 - Tamanho: " << img1.size() << ", Tipo: " << img1.type() << std::endl;
    std::cout << "Imagem 2 - Tamanho: " << img2.size() << ", Tipo: " << img2.type() << std::endl;

    // Verifica se as duas imagens têm o mesmo tamanho
    if (img1.size() != img2.size()) {
        // Caso os tamanhos sejam diferentes, redimensiona a imagem 2 para o tamanho da imagem 1
        std::cout << "Redimensionando a imagem 2 para o tamanho da imagem 1..." << std::endl;
        cv::resize(img2, img2, img1.size());  // Redimensiona img2 para o tamanho de img1
    }

    // Verifica se as duas imagens têm o mesmo número de canais (por exemplo, 3 canais para colorido, 1 canal para escala de cinza)
    if (img1.channels() != img2.channels()) {
        // Caso o número de canais seja diferente, converte ambas as imagens para o mesmo número de canais (escala de cinza)
        std::cout << "Convertendo para o mesmo número de canais..." << std::endl;
        if (img1.channels() == 3) {
            // Se img1 é colorida (3 canais), converte img2 para escala de cinza
            cv::cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
        } else {
            // Se img1 é em escala de cinza, converte img1 para escala de cinza
            cv::cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
        }
    }

    // Subtrai as duas imagens pixel a pixel e armazena o resultado em 'diff'
    cv::Mat diff;
    cv::absdiff(img1, img2, diff);  // absdiff calcula a diferença absoluta entre img1 e img2

    // Salva a imagem resultante da diferença em um arquivo
    bool sucesso = cv::imwrite("../imgs/dif3.2.png", diff);
    if (!sucesso) {
        // Se não foi possível salvar a imagem, exibe um erro
        std::cerr << "Erro ao salvar a imagem de diferença!" << std::endl;
        return -1;
    }

    // Exibe a imagem de diferença em uma janela
    cv::imshow("Imagem de Diferenca", diff);
    cv::waitKey(0);  // Aguarda o usuário pressionar qualquer tecla para fechar a janela

    return 0;  // Finaliza o programa
}
