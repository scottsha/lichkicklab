project(birth)

add_executable(${PROJECT_NAME} birth.cpp)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})