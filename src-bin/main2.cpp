#include <iostream>
#include <opencv2/opencv.hpp>

// Função para realizar interpolação bilinear em uma imagem
cv::Vec3b bilinearInterpolate(const cv::Mat& image, float x, float y) {
    // Obtém as coordenadas inteiras mais próximas ao ponto (x, y)
    int x1 = std::floor(x);  
    int y1 = std::floor(y);
    
    // Calcula as coordenadas do ponto superior direito e inferior direito, garantindo que não ultrapassem os limites da imagem
    int x2 = std::min(x1 + 1, image.cols - 1); 
    int y2 = std::min(y1 + 1, image.rows - 1); 

    // Diferença entre o ponto (x, y) e a coordenada inferior esquerda
    float dx = x - x1;
    float dy = y - y1;

    // Obtém os valores de cor nos quatro pontos ao redor do ponto de interpolação
    cv::Vec3b p1 = image.at<cv::Vec3b>(y1, x1);
    cv::Vec3b p2 = image.at<cv::Vec3b>(y1, x2);
    cv::Vec3b p3 = image.at<cv::Vec3b>(y2, x1);
    cv::Vec3b p4 = image.at<cv::Vec3b>(y2, x2);

    // Vetor para armazenar o resultado da interpolação
    cv::Vec3b result;

    // Interpola para cada canal de cor (R, G, B)
    for (int c = 0; c < 3; ++c) {
        // Interpolação bilinear entre os pontos horizontais (p1 e p2) e verticais (p3 e p4)
        float top = p1[c] * (1 - dx) + p2[c] * dx;
        float bottom = p3[c] * (1 - dx) + p4[c] * dx;
        result[c] = cv::saturate_cast<uchar>(top * (1 - dy) + bottom * dy); // Interpolação final e saturação para valores válidos de pixel
    }

    // Retorna o valor interpolado da cor do pixel
    return result;
}

int main() {
    // Caminho da imagem original
    std::string imagePath = "../imgs/2.1.jpg";

    // Carrega a imagem em cores
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    // Verifica se a imagem foi carregada corretamente
    if (image.empty()) {
        std::cerr << "Error" << std::endl;
        return -1;
    }

    // Define a resolução DPI (dots per inch) original da imagem
    int currentDpi = 300;

    // Define a resolução DPI desejada para a imagem
    int desiredDpi = 60;
    
    // Calcula o fator de escala entre a resolução original e a desejada
    float scalingFactor = (float)desiredDpi / currentDpi;

    // Calcula as novas dimensões da imagem com base no fator de escala
    int newWidth = (int)(image.cols * scalingFactor);
    int newHeight = (int)(image.rows * scalingFactor);

    // Cria uma nova imagem com as novas dimensões (sem interpolação)
    cv::Mat resizedImage(newHeight, newWidth, image.type());

    // Copia a parte relevante da imagem para a imagem redimensionada
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            // Calcula as coordenadas do pixel original correspondentes ao novo pixel (sem interpolação)
            int srcX = (int)(x / scalingFactor);
            int srcY = (int)(y / scalingFactor);

            // Copia o valor do pixel da imagem original para a imagem redimensionada
            resizedImage.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>(srcY, srcX);
        }
    }

    // Exibe a imagem original
    cv::namedWindow("DPI original", cv::WINDOW_NORMAL);
    cv::imshow("DPI original", image);

    // Exibe a imagem com o DPI reduzido (sem interpolação)
    cv::namedWindow("DPI reduzido", cv::WINDOW_NORMAL);
    cv::imshow("DPI reduzido", resizedImage);

    // Calcula o fator de escala inverso para o aumento da imagem
    float inverseScalingFactor = 1.0f / scalingFactor;

    // Calcula as novas dimensões da imagem aumentada
    int enlargedWidth = (int)(resizedImage.cols * inverseScalingFactor);
    int enlargedHeight = (int)(resizedImage.rows * inverseScalingFactor);

    // Cria uma nova imagem com as novas dimensões (aumentada)
    cv::Mat enlargedImage(enlargedHeight, enlargedWidth, resizedImage.type());

    // Realiza a interpolação bilinear para aumentar a imagem
    for (int y = 0; y < enlargedHeight; ++y) {
        for (int x = 0; x < enlargedWidth; ++x) {
            // Calcula as coordenadas do pixel original correspondentes ao novo pixel
            float srcX = x / inverseScalingFactor;
            float srcY = y / inverseScalingFactor;

            // Aplica a interpolação bilinear e atribui o valor ao pixel da imagem aumentada
            enlargedImage.at<cv::Vec3b>(y, x) = bilinearInterpolate(resizedImage, srcX, srcY);
        }
    }

    // Exibe a imagem com o DPI interpolado
    cv::namedWindow("DPI interpolado", cv::WINDOW_NORMAL);
    cv::imshow("DPI interpolado", enlargedImage);

    // Aguarda o usuário pressionar qualquer tecla
    cv::waitKey(0);

    // Salva as imagens redimensionada e aumentada em arquivos
    cv::imwrite("../imgs/3.2.tif", resizedImage);
    cv::imwrite("../imgs/3.3.tif", enlargedImage);

    return 0;
}
