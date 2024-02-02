cmake_minimum_required(VERSION 3.15)

project(Project)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin)
endforeach(OUTPUTCONFIG)

add_subdirectory(modules)

if(MSVC)
    add_compile_definitions(NOMINMAX)
endif()

set(CMAKE_CXX_STANDARD 20)
add_library(cpp_midi INTERFACE)
target_include_directories(cpp_midi INTERFACE include)
target_link_libraries(cpp_midi INTERFACE rtmidi)

add_executable(Canon src/Canon.cpp)
add_executable(Greatest_Work_of_Art src/Greatest_Work_of_Art.cpp)
add_executable(Piano src/Piano.cpp)
add_executable(Decorator src/Decorator.cpp)
add_executable(Streams src/Streams.cpp)

target_link_libraries(Canon cpp_midi)
target_link_libraries(Greatest_Work_of_Art cpp_midi)
target_link_libraries(Piano cpp_midi)
target_link_libraries(Decorator cpp_midi)
target_link_libraries(Streams cpp_midi)
