cmake_minimum_required(VERSION 3.10)
project(btn)

find_package(OpenCV REQUIRED)
include_directories(
    ../include
    ${OpenCV_INCLUDE_DIRS}
)

add_executable(adjustBrightnessContrast adjustBrightnessContrast.cpp)
target_link_libraries(adjustBrightnessContrast ${OpenCV_LIBS})

add_executable(imageCrop imageCrop.cpp)
target_link_libraries(imageCrop ${OpenCV_LIBS})

add_executable(adjustLevels adjustLevels.cpp ../src/Levels.cpp)
target_link_libraries(adjustLevels ${OpenCV_LIBS})

add_executable(adjustCurves adjustCurves.cpp ../src/Curves.cpp)
target_link_libraries(adjustCurves ${OpenCV_LIBS})

add_executable(adjustHSL adjustHSL.cpp ../src/HSL.cpp ../src/ColorSpace.cpp)
target_link_libraries(adjustHSL ${OpenCV_LIBS})

add_executable(selectiveColor selectiveColor.cpp ../src/SelectiveColor.cpp)
target_link_libraries(selectiveColor ${OpenCV_LIBS})

add_executable(adjustHighPass adjustHighPass.cpp ../src/Filter.cpp)
target_link_libraries(adjustHighPass ${OpenCV_LIBS})

add_executable(imageRotate imageRotate.cpp)
target_link_libraries(imageRotate ${OpenCV_LIBS})

add_executable(adjustBlackWhite adjustBlackWhite.cpp ../src/BlackWhite.cpp)
target_link_libraries(adjustBlackWhite ${OpenCV_LIBS})