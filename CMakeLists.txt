cmake_minimum_required(VERSION 3.9)
project(SimulationCourse)

set(CompilerFlags 
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
)
if (MSVC)
    foreach (CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        string(REPLACE "/W3" "/W4" ${CompilerFlag} "${${CompilerFlag}}")
        set(CompilerFlag "${CompilerFlag} /std:c++17")
    endforeach()
    set(SMFL_STATIC_LIBRAIRIES TRUE)
    set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -std=c++17")
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
find_package(SFML COMPONENTS graphics window audio system REQUIRED)

set(SRC "src/main.cpp")
set(LIBS "lib/coureur.cpp" "lib/parcours.cpp" "lib/simulation.cpp")
add_library(SimulationCourseLibs ${LIBS})
add_executable(SimulationCourse ${SRC})
target_link_libraries(SimulationCourse SimulationCourseLibs sfml-graphics sfml-window sfml-audio pthread)
