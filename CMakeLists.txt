cmake_minimum_required(VERSION 3.15)
project(ARINC818Simulator VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg манифест режим автоматически активируется при наличии vcpkg.json
# vcpkg указывается через CMAKE_TOOLCHAIN_FILE при вызове cmake

# Поиск библиотек, установленных через vcpkg
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# Опции компиляции
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")

# Создание исполняемого файла
add_executable(arinc818_sim 
    src/main.cpp
)

# Подключение найденных библиотек
target_link_libraries(arinc818_sim 
    PRIVATE 
    fmt::fmt 
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Добавляем определение для демонстрации уязвимостей (опционально)
if(WITH_VULNERABILITIES)
    target_compile_definitions(arinc818_sim PRIVATE ENABLE_VULN_DEMO)
    message(STATUS "Building with security vulnerability demonstrations")
endif()

# Настройка путей для Windows
if(WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(arinc818_sim ${CMAKE_THREAD_LIBS_INIT})
endif()

if(UNIX)
    target_link_libraries(arinc818_sim pthread)
endif()

# Установка свойств
set_target_properties(arinc818_sim PROPERTIES
    OUTPUT_NAME "arinc818_sim"
    VERSION ${PROJECT_VERSION}
)

# Добавляем пользовательскую команду для вывода информации о vcpkg
add_custom_command(TARGET arinc818_sim POST_BUILD
    COMMENT "Built with vcpkg dependencies: fmt, nlohmann-json, openssl"
    COMMAND ${CMAKE_COMMAND} -E echo "vcpkg integration enabled"
)