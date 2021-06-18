add_library(binproto
		src/Array.cpp
		src/BufferReader.cpp
		src/BufferWriter.cpp
		src/Version.cpp
		)

target_include_directories(binproto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TODO: Add modern-cmake stuff