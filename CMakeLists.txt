cmake_minimum_required(VERSION 2.6)
project(ogsfb32)

find_package(PkgConfig REQUIRED)
pkg_check_modules(BSD REQUIRED libbsd)
pkg_check_modules(DRM REQUIRED libdrm)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#--------------------------------------------------------------------------

add_library(ogsfb32 STATIC libogsfb32/drmUtil.cxx
                           libogsfb32/fileDescriptor.cxx
                           libogsfb32/framebuffer8880.cxx
                           libogsfb32/image8880.cxx
                           libogsfb32/image8880Font.cxx
                           libogsfb32/image8880Graphics.cxx
                           libogsfb32/joystick.cxx
                           libogsfb32/rgb8880.cxx)

include_directories(${PROJECT_SOURCE_DIR}/libogsfb32)
target_include_directories(ogsfb32 PUBLIC ${DRM_INCLUDE_DIRS})
target_compile_options(ogsfb32 PUBLIC ${DRM_CFLAGS_OTHER})
target_link_libraries(ogsfb32 ${DRM_LIBRARIES})

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

target_link_libraries(ogsinfo ogsfb32 ${BSD_LIBRARIES} ${DRM_LIBRARIES})
target_include_directories(ogsinfo PUBLIC ${BSD_INCLUDE_DIRS})
target_compile_options(ogsinfo PUBLIC ${BSD_CFLAGS_OTHER})

set_property(TARGET ogsinfo PROPERTY SKIP_BUILD_RPATH TRUE)
install (TARGETS ogsinfo RUNTIME DESTINATION bin)

#--------------------------------------------------------------------------

add_executable(boxworld boxworld/main.cxx
                        boxworld/images.cxx
                        boxworld/level.cxx
                        boxworld/levels.cxx
                        boxworld/boxworld.cxx)

target_link_libraries(boxworld ogsfb32 ${DRM_LIBRARIES})

#--------------------------------------------------------------------------

add_executable(life life/main.cxx
                    life/life.cxx)

target_link_libraries(life ogsfb32 ${DRM_LIBARARIES})

#--------------------------------------------------------------------------

add_executable(ogsfb32test test/test.cxx)
target_link_libraries(ogsfb32test ogsfb32 ${DRM_LIBARARIES})

add_executable(joysticktest test/testJoystick.cxx)
target_link_libraries(joysticktest ogsfb32)

