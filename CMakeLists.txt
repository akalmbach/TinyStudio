cmake_minimum_required(VERSION 2.6)
project (TinyStudio)
 
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

find_package( OpenCV REQUIRED )

set(TINYSTUDIO_SOURCES "${PROJECT_SOURCE_DIR}/src/TinyStudio.cpp"
                        "${PROJECT_SOURCE_DIR}/src/VideoProcessNode.cpp"
)


include_directories(${OpenCV_INCLUDE_DIRS}
                    ${PROJECT_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}
)
                    

add_executable(TinyStudio ${TINYSTUDIO_SOURCES})
target_link_libraries( TinyStudio ${OpenCV_LIBS} )
