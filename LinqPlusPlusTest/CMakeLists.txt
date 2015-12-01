project (LinqPlusPlusTest)

set(SOURCE_FILES main.cpp
                 EnumerableTest.cpp
                 EnumeratorTest.cpp)

find_package(Threads)

include(../cmake/googlemock.cmake)

include_directories(${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

add_dependencies(${PROJECT_NAME} LinqPlusPlus googlemock)

target_link_libraries(${PROJECT_NAME} LinqPlusPlus ${GTEST_LIB} ${GMOCK_LIB} ${CMAKE_THREAD_LIBS_INIT})

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}"
)
