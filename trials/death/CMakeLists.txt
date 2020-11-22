project(death)

add_executable(${PROJECT_NAME} death.cpp)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})