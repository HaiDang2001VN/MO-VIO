add_executable(movio main.cpp)
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)
TARGET_LINK_LIBRARIES(movio ${OpenCV_LIBS}) 
