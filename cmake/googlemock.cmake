include(ExternalProject)

ExternalProject_Add(
        googlemock
        GIT_REPOSITORY https://github.com/google/googletest.git
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
)

ExternalProject_Get_Property(googlemock source_dir binary_dir)

set(GMOCK_INCLUDE_DIR ${source_dir}/googlemock/include)
set(GTEST_INCLUDE_DIR ${source_dir}/googletest/include)

set(GMOCK_LIB ${binary_dir}/googlemock/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX})
set(GTEST_LIB ${binary_dir}/googlemock/gtest/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})
set(GMOCK_MAIN_LIB ${binary_dir}/googlemock/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_main${CMAKE_STATIC_LIBRARY_SUFFIX})
set(GTEST_MAIN_LIB ${binary_dir}/googlemock/gtest/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})

include_directories(${GMOCK_INCLUDE_DIR})
