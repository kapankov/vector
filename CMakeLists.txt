cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(vector LANGUAGES CXX)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Test
if(MSVC)
	set(gtest_ROOT "C:/Program Files (x86)/gtest")
endif()

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME})
# add_library(${PROJECT_NAME} STATIC)
# add_library(${PROJECT_NAME} SHARED)

# CXX_STANDARD support:
# CMAKE 3.1 - 98,11 and 14
# CMAKE 3.8 - 17
# CMAKE 3.12 - 20
# CMAKE 3.20 - 23
# CMAKE 3.25 - 26
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		include/
#		second_dir/
)

set_target_properties(${PROJECT_NAME}
	PROPERTIES
    	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out
)

target_sources(${PROJECT_NAME}
	PRIVATE
	test/vector_test.cpp
	include/vector.h
#		src/src.cpp
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		gtest
		gtest_main
)
