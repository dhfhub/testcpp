cmake_minimum_required(VERSION 3.10)
project(btn)

find_package(OpenCV REQUIRED)
include_directories(
    include
    ${OpenCV_INCLUDE_DIRS}
)

add_executable(btn src/main.cpp src/preProcessImage.cpp src/Curves.cpp)
target_link_libraries(btn ${OpenCV_LIBS})