add_executable(lydia-server
		src/main.cpp
		)
target_include_directories(lydia-server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(lydia-server binproto lydia-protocol narwhal)
