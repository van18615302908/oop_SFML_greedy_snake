cmake_minimum_required(VERSION 3.16)

project(.)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

#set(SFML_ROOT "C:/Users/admin/Desktop/sfSnakePro/SFML-2.5.1")
set(SFML_ROOT "../../SFML-2.5.1")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(main ${SOURCES})
target_include_directories(main PUBLIC src thirdparty/SFML/include)
target_link_libraries(main PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)
set_property(TARGET main PROPERTY
        CXX_STANDARD          17
        CXX_STANDARD_REQUIRED ON)

file(COPY "assets" DESTINATION "${CMAKE_BINARY_DIR}/${NAME}")
