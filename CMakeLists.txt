cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
SET(CMAKE_PREFIX_PATH "/home/khaitq/Downloads/tesseract-main/cmake")
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} ) 
add_executable(main main.cpp )
target_link_libraries(main xdo ${OpenCV_LIBS} tesseract opencv_highgui X11 )

