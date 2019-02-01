cmake_minimum_required(VERSION 3.13)
project(SFMLTest)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")


add_executable(SFMLTest ${SOURCE_FILES})
set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window audio)

target_link_libraries(SFMLTest sfml-graphics sfml-window sfml-audio)