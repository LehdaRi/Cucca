cmake_minimum_required(VERSION 2.8)


#sources
file(GLOB cucca_graphics_SRC "*.cpp")
add_library(cucca-graphics SHARED ${cucca_graphics_SRC})


#libraries
#OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

#GLEW
find_path(GLEW_INCLUDE_DIR GL/glew.h)
find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64)
include_directories(${GLEW_INCLUDE_DIR})
add_definitions(-DGLEW_STATIC)


target_link_libraries(cucca-graphics cucca-core ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})


#link debug
if(CUCCA_BUILD_DEBUG)
    target_link_libraries(cucca-graphics cucca-debug)
endif()


#compilation options
#suppress Eigen deprecation warnings
target_compile_options(cucca-graphics PRIVATE -Wno-deprecated-declarations)


#install
install (TARGETS cucca-graphics     DESTINATION lib)
