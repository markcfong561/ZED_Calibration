cmake_minimum_required(VERSION 2.8)
project( ZedCalibration )
set(CMAKE_CXX_FLAGS "-std=c++11")
SET("OpenCV_DIR" "/usr/share/OpenCV")
find_package(OpenCV REQUIRED)
find_package(ZED 4 REQUIRED)
find_package(CUDA ${ZED_CUDA_VERSION} EXACT REQUIRED)

include_directories( include ${OpenCV_INCLUDE_DIRS} ${ZED_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS})

add_executable( zedCollection src/zedCollection.cpp )
target_link_libraries( zedCollection ${OpenCV_LIBS} ${ZED_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY})

add_executable( zedCalibration src/zedCalibration.cpp )
target_link_libraries( zedCalibration  ${OpenCV_LIBS} ${ZED_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY})