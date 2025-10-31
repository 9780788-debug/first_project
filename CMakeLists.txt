cmake_minimum_required(VERSION 3.10)

# 1. Вказуємо мову CXX (C++)
project(MyfirstGame LANGUAGES CXX)

# Використовуємо стандарт C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Знаходимо SFML
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Створюємо нашу гру
add_executable(MyfirstGame MyFirstGame.cpp)

# 2. Прив'язуємо бібліотеки SFML + стандартну бібліотеку C++ (stdc++)
target_link_libraries(MyfirstGame PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    stdc++
)