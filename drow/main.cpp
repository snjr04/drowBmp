#include <fstream>
#include <iostream>
#include <vector>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t file_type{ 0x4D42 };  
    uint32_t file_size{ 0 };       
    uint16_t reserved1{ 0 };       
    uint16_t reserved2{ 0 };       
    uint32_t offset_data{ 0 };     
};

struct BMPInfoHeader {
    uint32_t size{ 0 };            
    int32_t width{ 0 };            
    int32_t height{ 0 };           
    uint16_t planes{ 1 };          
    uint16_t bit_count{ 24 };      
    uint32_t compression{ 0 };     
    uint32_t size_image{ 0 };      
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };     
    uint32_t colors_important{ 0 }; 
};
#pragma pack(pop)

struct Color {
    uint8_t b, g, r;  
};

void printBMPImage(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "ќшибка открыти€ файла!" << std::endl;
        return;
    }

    BMPHeader bmpHeader;
    BMPInfoHeader bmpInfoHeader;

    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
    file.read(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));

    if (bmpInfoHeader.bit_count != 24) {
        std::cerr << "‘айл не €вл€етс€ 24-битным BMP изображением!" << std::endl;
        return;
    }

    file.seekg(bmpHeader.offset_data, file.beg);
    int row_stride = (bmpInfoHeader.width * 3 + 3) & ~3;
    std::vector<Color> imageData(bmpInfoHeader.width);


    for (int y = 0; y < bmpInfoHeader.height; ++y) {
        file.read(reinterpret_cast<char*>(imageData.data()), bmpInfoHeader.width * sizeof(Color));


        for (int x = 0; x < bmpInfoHeader.width; ++x) {
            Color& pixel = imageData[x];
            if (pixel.r == 0 && pixel.g == 0 && pixel.b == 0) {
                std::cout << "#";  // ѕиксель черного цвета 
            }
            else if (pixel.r == 255 && pixel.g == 255 && pixel.b == 255) {
                std::cout << "*";  // ѕиксель белого цвета 
            }
            else {
                std::cout << "%";  // ѕиксель серого цвета, без этого почему то не работает извин€юсь за баг но это фича 
            }
        }
        std::cout << std::endl;
        file.seekg(row_stride - bmpInfoHeader.width * sizeof(Color), std::ios::cur);
    }

    file.close();
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::string filePath = "C:\\Work\\test.bmp";//путь к bmp файлу.
    printBMPImage(filePath);
    return 0;
}
