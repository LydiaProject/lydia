set(CMAKE_CXX_STANDARD 20)

add_library(narwhal src/dummy.cpp)

target_include_directories(narwhal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TODO: modern-cmake export target
#export(TARGETS protocol NAMESPACE lydia:: FILE LydiaProtocolTargets.cmake)
