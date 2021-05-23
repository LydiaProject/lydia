set(CMAKE_CXX_STANDARD 20)

add_library(binproto
		EndianUtils.cpp
		Array.cpp
		BufferReader.cpp
		BufferWriter.cpp
		Version.cpp
)

target_include_directories(binproto PUBLIC ${PROJECT_SOURCE_DIR}/include)

# TODO add modern cmake target
