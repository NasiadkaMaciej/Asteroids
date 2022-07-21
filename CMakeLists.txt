
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

IF(WIN32)
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE) # vcpkg path
    set(VCPKG_TARGET_TRIPLET "x86-windows-static" CACHE STRING "" FORCE)
ENDIF()

set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

project(Asteroids
        LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
    set(target_compile_options("-Wall"))
    set(VCPKG_LIBRARY_LINKAGE static)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio CONFIG REQUIRED)
find_package(CURL REQUIRED)

file(GLOB SRC
    src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    CURL::libcurl)
