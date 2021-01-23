project(primecheck)

add_executable(${PROJECT_NAME} prime_check_mit_pretest.cpp)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

add_executable(assign1 assign1.cpp)
