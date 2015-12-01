include(ExternalProject)

ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
)

ExternalProject_Get_Property(googletest source_dir binary_dir)

message("GTEST_BINARY_DIR: ${binary_dir}")
message("GTEST_SOURCE_DIR: ${source_dir}")

set(GTEST_INCLUDE_DIR ${source_dir}/googletest/include)
set(GTEST_LIB ${binary_dir}/googlemock/gtest/libgtest.a)
set(GTEST_MAIN_LIB ${binary_dir}/libgtest_main.a)

include_directories(${GTEST_INCLUDE_DIR})
