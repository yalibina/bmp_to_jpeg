cmake_minimum_required(VERSION 3.22)
project(bmp_to_jpeg)

set(CMAKE_CXX_STANDARD 17)

add_executable(project
        main.cpp
        image/image.cpp
        image/pixel.cpp
        processing/file_process.cpp
        processing/subsample.cpp
        processing/matrix.tpp
        processing/discrete_cosine_transform.cpp
        processing/quantization.cpp)