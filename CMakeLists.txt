cmake_minimum_required(VERSION 2.8)
project( valueFinder C CXX)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_ADDITIONAL_VERSIONS "1.48" "1.46")

find_package( Boost REQUIRED COMPONENTS thread )
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

find_package(Threads REQUIRED)

#add_library( f0 f0.o)
#SET_TARGET_PROPERTIES(f0 PROPERTIES LINKER_LANGUAGE C++)
#link_libraries(f0)

#add_library(Utility include/utility.cpp)
#target_link_libraries(Utility ${catkin_LIBRARIES})
#link_libraries(Utility)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-o ../f1 ../f1.o")

#add_custom_command(TARGET test PRE_LINK "-o f0.o")

add_library(species species.cpp)
target_link_libraries(species ${catkin_LIBRARIES})
link_libraries(species)

add_library(individual individual.cpp)
target_link_libraries(individual ${catkin_LIBRARIES})
link_libraries(individual)

add_library(eval eval.cpp)
target_link_libraries(eval ${catkin_LIBRARIES})
link_libraries(eval)

add_library(utility utility.cpp)
target_link_libraries(utility ${catkin_LIBRARIES})
link_libraries(utility)

add_executable( main main.cpp )
target_link_libraries( main ${Boost_LIBRARIES} )

add_executable( test test.cpp )
target_link_libraries( test ${Boost_LIBRARIES} )
