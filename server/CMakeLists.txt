cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

add_executable(lydia-server
		src/main.cpp
		)
target_include_directories(lydia-server PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(lydia-server binproto lydia-protocol narwhal)
