project(LinqPlusPlus)

set(SOURCE_FILES
    include/LinqPlusPlus/Enumerable.h
    include/LinqPlusPlus/IEnumerable.h
    include/LinqPlusPlus/Enumerators/ArrayEnumerator.h
    include/LinqPlusPlus/Enumerators/Combine.h
    include/LinqPlusPlus/Enumerators/ContainerEnumerator.h
    include/LinqPlusPlus/Enumerators/Enumerator.h
    include/LinqPlusPlus/Enumerators/Filter.h
    include/LinqPlusPlus/Enumerators/Map.h
    include/LinqPlusPlus/Enumerators/SequenceGenerator.h
    include/LinqPlusPlus/Exceptions/ArgumentNullException.h
    src/Exceptions/ArgumentNullException.cpp
 )

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
