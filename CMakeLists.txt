cmake_minimum_required(VERSION 3.15)
project(ARINC818Simulator VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для CI - пропускать поиск пакетов если они не найдены
option(SKIP_PACKAGE_CHECK "Skip checking for packages" OFF)

if(NOT SKIP_PACKAGE_CHECK)
    # Поиск пакетов (только если не пропускаем)
    find_package(fmt QUIET)
    find_package(nlohmann_json QUIET)
    find_package(OpenSSL QUIET)
endif()

# Создание исполняемого файла
add_executable(arinc818_sim
    src/main.cpp
)

# Подключаем библиотеки только если они найдены
if(fmt_FOUND)
    target_link_libraries(arinc818_sim PRIVATE fmt::fmt)
endif()

if(nlohmann_json_FOUND)
    target_link_libraries(arinc818_sim PRIVATE nlohmann_json::nlohmann_json)
endif()

if(OPENSSL_FOUND)
    target_link_libraries(arinc818_sim PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()

# Для Windows
if(WIN32)
    target_link_libraries(arinc818_sim ${CMAKE_THREAD_LIBS_INIT})
endif()

if(UNIX AND NOT WIN32)
    target_link_libraries(arinc818_sim pthread)
endif()