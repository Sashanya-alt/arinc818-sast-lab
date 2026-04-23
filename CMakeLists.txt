cmake_minimum_required(VERSION 3.15)
project(ARINC818Simulator VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создание исполняемого файла
add_executable(arinc818_sim
    src/main.cpp
)

# Подключение заголовков
target_include_directories(arinc818_sim PRIVATE include)

# Базовые флаги компиляции
target_compile_options(arinc818_sim PRIVATE -Wall -Wextra)

# Подключение библиотек (упрощённо)
if(UNIX)
    target_link_libraries(arinc818_sim pthread)
endif()

# Опционально: если библиотеки установлены в системе
find_package(fmt QUIET)
if(fmt_FOUND)
    target_link_libraries(arinc818_sim fmt::fmt)
endif()

find_package(nlohmann_json QUIET)
if(nlohmann_json_FOUND)
    target_link_libraries(arinc818_sim nlohmann_json::nlohmann_json)
endif()

find_package(OpenSSL QUIET)
if(OPENSSL_FOUND)
    target_link_libraries(arinc818_sim OpenSSL::SSL OpenSSL::Crypto)
endif()