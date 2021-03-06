cmake_minimum_required(VERSION 2.8)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project(Cucca)


include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


option (CUCCA_BUILD_DEBUG "Build debug build" ON)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories("${CMAKE_SOURCE_DIR}/include")
#configure_file("${PROJECT_SOURCE_DIR}/include/CMakeConfig.hpp.in"
#               "${PROJECT_BINARY_DIR}/include/CMakeConfig.hpp")

if (CUCCA_BUILD_DEBUG)
    add_definitions(-DCUCCA_DEBUG)
    add_subdirectory(src/Debug)
endif (CUCCA_BUILD_DEBUG)


add_subdirectory(src/Core)
add_subdirectory(src/CoreExtensions)
add_subdirectory(src/Graphics)
add_subdirectory(src/GraphicsExtensions)


file(GLOB cucca_core_INCLUDE "${CMAKE_SOURCE_DIR}/include/Core/*.hpp")
install(FILES ${cucca_core_INCLUDE} DESTINATION include/Cucca/Core)
file(GLOB cucca_coreext_INCLUDE "${CMAKE_SOURCE_DIR}/include/CoreExtensions/*.hpp")
install(FILES ${cucca_coreext_INCLUDE} DESTINATION include/Cucca/CoreExtensions)
file(GLOB cucca_graphics_INCLUDE "${CMAKE_SOURCE_DIR}/include/Graphics/*.hpp")
install(FILES ${cucca_graphics_INCLUDE} DESTINATION include/Cucca/Graphics)
file(GLOB cucca_graphicsext_INCLUDE "${CMAKE_SOURCE_DIR}/include/GraphicsExtensions/*.hpp")
install(FILES ${cucca_graphicsext_INCLUDE} DESTINATION include/Cucca/GraphicsExtensions)
if (CUCCA_BUILD_DEBUG)
	file(GLOB cucca_debug_INCLUDE "${CMAKE_SOURCE_DIR}/include/Debug/*.hpp")
	install(FILES ${cucca_debug_INCLUDE} DESTINATION include/Cucca/Debug)
endif (CUCCA_BUILD_DEBUG)
