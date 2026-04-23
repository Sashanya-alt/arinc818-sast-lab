#ifndef ARINC818_SIMULATOR_H
#define ARINC818_SIMULATOR_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <chrono>
#include <thread>
#include <random>
#include <functional>
#include <map>

// Параметры видео
enum class VideoFormat {
    VGA_640x480,
    HD_1280x720,
    FULL_HD_1920x1080
};

enum class PixelFormat {
    RGB_888,
    YCbCr_422
};

// Структура кадра
struct ARINCFrame {
    uint32_t frame_number;
    uint64_t timestamp;
    VideoFormat format;
    PixelFormat pixel_format;
    uint32_t width;
    uint32_t height;
    uint32_t line_count;
    std::vector<uint8_t> video_data;
    std::vector<uint32_t> crc_values;
    uint32_t frame_crc;
    
    ARINCFrame() : frame_number(0), timestamp(0), width(0), height(0), 
                   line_count(0), frame_crc(0) {}
};

// Структура пакета
struct ARINCPacket {
    int type;
    uint32_t sequence_num;
    uint32_t line_num;
    std::vector<uint8_t> payload;
    uint32_t crc;
};

// Простой генератор видео
class VideoGenerator {
public:
    std::vector<uint8_t> generateTestPattern(VideoFormat format, PixelFormat pix_fmt) {
        uint32_t width = 640, height = 480;
        if (format == VideoFormat::HD_1280x720) { width = 1280; height = 720; }
        if (format == VideoFormat::FULL_HD_1920x1080) { width = 1920; height = 1080; }
        
        uint32_t bytes_per_pixel = (pix_fmt == PixelFormat::RGB_888) ? 3 : 2;
        uint32_t frame_size = width * height * bytes_per_pixel;
        
        std::vector<uint8_t> frame_data(frame_size);
        
        for (uint32_t i = 0; i < frame_size; i++) {
            frame_data[i] = static_cast<uint8_t>(i % 256);
        }
        
        return frame_data;
    }
};

// Простой CRC калькулятор
class CRC32Calculator {
public:
    uint32_t calculate(const uint8_t* data, size_t length) {
        uint32_t crc = 0xFFFFFFFF;
        for (size_t i = 0; i < length; i++) {
            crc ^= data[i];
            for (int j = 0; j < 8; j++) {
                crc = (crc >> 1) ^ ((crc & 1) ? 0xEDB88320 : 0);
            }
        }
        return ~crc;
    }
};

// Передатчик
class ARINC818Transmitter {
private:
    CRC32Calculator crc_calc;
    VideoGenerator video_gen;
    uint32_t frame_counter;
    
public:
    ARINC818Transmitter() : frame_counter(0) {}
    
    ARINCFrame createFrame(VideoFormat format, PixelFormat pix_fmt) {
        ARINCFrame frame;
        frame.frame_number = frame_counter++;
        frame.timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count();
        frame.format = format;
        frame.pixel_format = pix_fmt;
        
        if (format == VideoFormat::VGA_640x480) {
            frame.width = 640; frame.height = 480;
        } else if (format == VideoFormat::HD_1280x720) {
            frame.width = 1280; frame.height = 720;
        } else {
            frame.width = 1920; frame.height = 1080;
        }
        
        frame.video_data = video_gen.generateTestPattern(format, pix_fmt);
        frame.frame_crc = crc_calc.calculate(frame.video_data.data(), frame.video_data.size());
        frame.line_count = frame.height;
        
        return frame;
    }
    
    void transmitFrame(const ARINCFrame& frame, std::function<void(const ARINCPacket&)> channel) {
        ARINCPacket packet;
        packet.type = 1;
        packet.sequence_num = frame.frame_number;
        packet.payload = frame.video_data;
        packet.crc = frame.frame_crc;
        channel(packet);
    }
};

// Приемник
class ARINC818Receiver {
private:
    uint32_t received_frames;
    uint32_t error_count;
    std::map<uint32_t, ARINCFrame> frames;
    
public:
    ARINC818Receiver() : received_frames(0), error_count(0) {}
    
    bool processPacket(const ARINCPacket& packet) {
        ARINCFrame frame;
        frame.frame_number = packet.sequence_num;
        frame.video_data = packet.payload;
        frame.frame_crc = packet.crc;
        frames[received_frames] = frame;
        received_frames++;
        return true;
    }
    
    ARINCFrame getLastCompleteFrame() {
        if (received_frames > 0) {
            return frames[received_frames - 1];
        }
        return ARINCFrame();
    }
    
    void printStatistics() {
        std::cout << "\n=== Receiver Statistics ===" << std::endl;
        std::cout << "Received frames: " << received_frames << std::endl;
        std::cout << "Errors: " << error_count << std::endl;
    }
};

// Канал передачи
class TransmissionChannel {
public:
    TransmissionChannel(double err_rate, double loss) {}
    
    void transmit(const ARINCPacket& packet, std::function<void(const ARINCPacket&)> receiver) {
        receiver(packet);
    }
};

#endif