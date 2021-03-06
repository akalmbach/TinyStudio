cmake_minimum_required(VERSION 2.6)
project (TinyStudio)
 
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

MESSAGE("${CMAKE_MODULE_PATH}")

find_package( OpenCV REQUIRED )

add_subdirectory(${PROJECT_SOURCE_DIR}/pugixml)

set(TINYSTUDIO_SOURCES "${PROJECT_SOURCE_DIR}/src/TinyStudio.cpp"
                        "${PROJECT_SOURCE_DIR}/src/VideoProcessNode.cpp"
                        "${PROJECT_SOURCE_DIR}/src/VideoSource.cpp"
                        "${PROJECT_SOURCE_DIR}/src/VideoSink.cpp"
                        "${PROJECT_SOURCE_DIR}/src/VideoSplice.cpp"
                        "${PROJECT_SOURCE_DIR}/src/VideoSplit.cpp"
                        "${PROJECT_SOURCE_DIR}/src/VideoSequence.cpp"
                        "${PROJECT_SOURCE_DIR}/src/VideoSketch.cpp"
)


include_directories(${OpenCV_INCLUDE_DIRS}
                    ${PROJECT_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/pugixml/src
)

link_directories(${CMAKE_BINARY_DIR})
                    

add_executable(TinyStudio ${TINYSTUDIO_SOURCES})
target_link_libraries( TinyStudio ${OpenCV_LIBS} pugixml)
