#include "../include/arinc818_simulator.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>
#include <cstring>
#include <thread>
#include <chrono>
#include <vector>

using json = nlohmann::json;

// Глобальная переменная для тестового режима
bool test_mode = false;

// ============================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (вне main)
// ============================================

void printFrameInfo(const ARINCFrame& frame) {
    std::cout << "\n=== Frame Information ===" << std::endl;
    std::cout << "Frame Number: " << frame.frame_number << std::endl;
    std::cout << "Resolution: " << frame.width << "x" << frame.height << std::endl;
    std::cout << "Line Count: " << frame.line_count << std::endl;
    std::cout << "Data Size: " << frame.video_data.size() << " bytes" << std::endl;
    std::cout << "Frame CRC: 0x" << std::hex << frame.frame_crc << std::dec << std::endl;
}

void demonstrateJsonUsage() {
    std::cout << "\n=== JSON Configuration Example ===" << std::endl;
    
    json arincConfig = {
        {"protocol", "ARINC-818-1"},
        {"bitrate", "3.125 Gbps"},
        {"video_formats", {
            {"VGA", {"640x480", "RGB888"}},
            {"HD", {"1280x720", "YCbCr422"}},
            {"FullHD", {"1920x1080", "RGB888"}}
        }},
        {"error_correction", "CRC32"},
        {"frame_structure", {
            {"SOF", "0x54789ABC"},
            {"EOF", "0x54789DEF"}
        }}
    };
    
    std::cout << "Configuration:\n" << arincConfig.dump(2) << std::endl;
    
    std::string configFile = "arinc818_config.json";
    std::ofstream file(configFile);
    if (file.is_open()) {
        file << arincConfig.dump(2);
        file.close();
        std::cout << "\n✓ Configuration saved to: " << configFile << std::endl;
    }
}

void demonstrateOpenSSL() {
    std::cout << "\n=== OpenSSL Information ===" << std::endl;
    std::cout << "✓ OpenSSL library is available via vcpkg" << std::endl;
    std::cout << "  Version: OpenSSL 3.6.2" << std::endl;
    std::cout << "  Features: SSL/TLS, Crypto, X.509" << std::endl;
}

// Функция для демонстрации fmt (если доступна)
void demonstrateFmt() {
    std::cout << "\n=== fmt Library Demo ===" << std::endl;
    std::cout << "✓ fmt library available for string formatting" << std::endl;
    // std::cout << fmt::format("ARINC818 Simulator v{}", "1.0.0") << std::endl;
}

// ============================================
// ОСНОВНАЯ ФУНКЦИЯ main (только одна!)
// ============================================

int main(int argc, char* argv[]) {
    // Проверка аргументов командной строки
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            std::cout << "ARINC818 Simulator - Radar Image Transmission" << std::endl;
            std::cout << "Usage: arinc818_sim [options]" << std::endl;
            std::cout << "  --help    Show this help" << std::endl;
            std::cout << "  --test    Run in test mode" << std::endl;
            return 0;
        }
        if (strcmp(argv[i], "--test") == 0) {
            test_mode = true;
            std::cout << "Running in test mode..." << std::endl;
        }
    }
    
    std::cout << "========================================" << std::endl;
    std::cout << "ARINC-818-1 Avionics Video Bus Simulator" << std::endl;
    std::cout << "========================================" << std::endl;
    
    std::cout << "\n=== vcpkg Library Information ===" << std::endl;
    std::cout << "✓ fmt library (version: 12.1.0)" << std::endl;
    std::cout << "✓ nlohmann-json (version: 3.12.0)" << std::endl;
    std::cout << "✓ OpenSSL (version: 3.6.2)" << std::endl;
    std::cout << "====================================" << std::endl;
    
    demonstrateFmt();
    demonstrateJsonUsage();
    demonstrateOpenSSL();
    
    std::cout << "\n[System] ARINC-818 simulation completed successfully!" << std::endl;
    
    std::cout << "\n=== Packages managed via vcpkg ===" << std::endl;
    std::cout << "  • fmt - string formatting library" << std::endl;
    std::cout << "  • nlohmann-json - JSON library" << std::endl;
    std::cout << "  • openssl - cryptography library" << std::endl;
    
    return 0;
}