# “丧文化”不可推崇  
cmake_minimum_required(VERSION 3.2)
project(gal)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)

set(SRCS 
    main.cpp 
    ALGOLine.cpp 
    XScanner.cpp

    )
#aux_source_directory(. SRCS)
set(SDL2_DIR D:/softwares/SDL_lib/SDL2)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

include_directories(${SDL2_DIR}/include ${PROJECT_SOURCE_DIR})
link_directories(${SDL2_DIR}/lib)
#link_libraries(${SDL2_DIR}/lib)

add_executable(gal ${SRCS})

target_link_libraries(gal mingw32 SDL2main SDL2) 

