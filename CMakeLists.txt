cmake_minimum_required(VERSION 2.6)
project(Clover)

# project version
SET(${PROJECT_NAME}_MAJOR_VERSION 0)
SET(${PROJECT_NAME}_MINOR_VERSION 1)
SET(${PROJECT_NAME}_PATCH_LEVEL 0)
# clover version
SET(${PROJECT_NAME}_VERSION 0.1.0)
# OpenCL version
SET(${PROJECT_NAME}_SOVERSION 1.0.0)


OPTION(BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON)

add_subdirectory(src)
add_subdirectory(examples)
