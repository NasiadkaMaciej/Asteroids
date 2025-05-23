cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

#set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

project(Asteroids
		LANGUAGES CXX)

	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED True)
	set(target_compile_options("-Wall"))
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(VCPKG_LIBRARY_LINKAGE static)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

find_package(SFML 3.0.0 COMPONENTS Window Graphics Audio CONFIG REQUIRED)
find_package(CURL REQUIRED)
file(GLOB SRC
	src/*.cpp
	src/entities/*.cpp
	src/ui/*.cpp
	src/utils/*.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	sfml-graphics
	sfml-window
	sfml-system
	sfml-audio
	CURL::libcurl)
