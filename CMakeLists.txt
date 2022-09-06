cmake_minimum_required(VERSION 2.6)
project(ogsfb32)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#--------------------------------------------------------------------------

add_library(ogsfb32 STATIC libogsfb32/fileDescriptor.cxx
                           libogsfb32/framebuffer8880.cxx
                           libogsfb32/image8880.cxx
                           libogsfb32/image8880Font.cxx
                           libogsfb32/image8880Graphics.cxx
                           libogsfb32/joystick.cxx
                           libogsfb32/rgb8880.cxx)

include_directories(${PROJECT_SOURCE_DIR}/libogsfb32)

set(EXTRA_LIBS ${EXTRA_LIBS} ogsfb32)

#--------------------------------------------------------------------------

add_executable(ogsinfo ogsinfo/ogsinfo.cxx
                       ogsinfo/cpuTrace.cxx
                       ogsinfo/dynamicInfo.cxx
                       ogsinfo/memoryTrace.cxx
                       ogsinfo/networkTrace.cxx
                       ogsinfo/panel.cxx
                       ogsinfo/system.cxx
                       ogsinfo/temperatureTrace.cxx
                       ogsinfo/trace.cxx
                       ogsinfo/traceGraph.cxx
                       ogsinfo/traceStack.cxx)

target_link_libraries(ogsinfo ogsfb32 -lbsd)

set_property(TARGET ogsinfo PROPERTY SKIP_BUILD_RPATH TRUE)
install (TARGETS ogsinfo RUNTIME DESTINATION bin)

#--------------------------------------------------------------------------

add_executable(boxworld boxworld/main.cxx
                        boxworld/images.cxx
                        boxworld/level.cxx
                        boxworld/levels.cxx
                        boxworld/boxworld.cxx)

target_link_libraries(boxworld ogsfb32)

#--------------------------------------------------------------------------

add_executable(life life/main.cxx
                    life/life.cxx)

target_link_libraries(life ogsfb32)

#--------------------------------------------------------------------------

add_executable(ogsfb32test test/test.cxx)
target_link_libraries(ogsfb32test ogsfb32)

add_executable(joysticktest test/testJoystick.cxx)
target_link_libraries(joysticktest ogsfb32)

