add_library(lydia-protocol
		src/messages/ConnectMessage.cpp
		src/messages/ListMessage.cpp src/messages/VMReference.cpp include/lydia/messages/UserMessages.h src/messages/UserMessages.cpp include/lydia/messages/ControlMessages.h src/messages/ControlMessages.cpp)

target_include_directories(lydia-protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(lydia-protocol binproto narwhal)

# TODO: modern-cmake export target
#export(TARGETS protocol NAMESPACE lydia:: FILE LydiaProtocolTargets.cmake)
