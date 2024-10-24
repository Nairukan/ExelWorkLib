cmake_minimum_required(VERSION 3.14)

project(ExelWorkLib LANGUAGES CXX)

if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_VERSION 1.0)
#option(C OFF)

#find_package(PkgConfig REQUIRED)
find_package(fmt REQUIRED)
find_package(libzip REQUIRED)

add_library(${PROJECT_NAME}_static STATIC)
add_library(${PROJECT_NAME}_shared SHARED)

target_sources(${PROJECT_NAME}_static
    PRIVATE
        include/exelworklib.h
        src/exelworklib.cpp
)

target_sources(${PROJECT_NAME}_shared
    PRIVATE
        include/exelworklib.h
        src/exelworklib.cpp
)

target_include_directories(${PROJECT_NAME}_static
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${LIBZIP_INCLUDE_DIR}
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_include_directories(${PROJECT_NAME}_shared
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${LIBZIP_INCLUDE_DIR}
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME}_static 
    PUBLIC 
        libzip::zip fmt::fmt
    PRIVATE
        -static-libgcc -static-libstdc++
)

target_link_libraries(${PROJECT_NAME}_shared 
    PUBLIC 
        libzip::zip fmt::fmt
)
#[[
# note that it is not CMAKE_INSTALL_PREFIX we are checking here
if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX is not set\n"
        "Default value: ${CMAKE_INSTALL_PREFIX}\n"
        "Will set it to ${CMAKE_SOURCE_DIR}/install"
    )
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/install"
        CACHE PATH "Where the library will be installed to" FORCE
    )
else()
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX was already set\n"
        "Current value: ${CMAKE_INSTALL_PREFIX}"
    )
endif()
#]]

#without it public headers won't get installed
set(public_headers
   include/exelworklib.h
   include/ExelWorkLib_global.h
)
# note that ${public_headers} has to be in quotes
set_target_properties(${PROJECT_NAME}_shared PROPERTIES PUBLIC_HEADER "${public_headers}")
set_target_properties(${PROJECT_NAME}_shared PROPERTIES DEBUG_POSTFIX "d")

set_target_properties(${PROJECT_NAME}_static PROPERTIES PUBLIC_HEADER "${public_headers}")
set_target_properties(${PROJECT_NAME}_static PROPERTIES DEBUG_POSTFIX "d")

set_property(TARGET ${PROJECT_NAME}_static PROPERTY POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)

# install the target and create export-set
install(TARGETS ${PROJECT_NAME}_static ${PROJECT_NAME}_shared
    EXPORT "${PROJECT_NAME}Targets"
    # these get default values from GNUInstallDirs, no need to set them
    #RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR} # bin
    #LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
    #ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
    # except for public headers, as we want them to be inside a library folder
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}#/${PROJECT_NAME} # include/SomeLibrary
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include
)

install(EXPORT "${PROJECT_NAME}Targets"
    FILE "${PROJECT_NAME}Targets.cmake"
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)

# generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
)
# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION cmake
)
# install config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION cmake
)
