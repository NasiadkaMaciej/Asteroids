
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

IF(WIN32)
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE) # Vcpkg path
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "" FORCE)
ENDIF()

set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

project(Asteroids
        LANGUAGES CXX
)

IF(MSVC)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
ELSE()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall" CACHE STRING "" FORCE)
ENDIF()

IF(WIN32)
    find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)

    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/SFML/include
    )
ELSE()
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/SFML/include
        #/u/usr/local/include/ # Dir of SFML
    )
ENDIF()

file(GLOB SRC
    *.cpp
)

IF(UNIX)
    link_directories(/usr/local/lib) # Dir of SFML
ENDIF(UNIX)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    curl)
