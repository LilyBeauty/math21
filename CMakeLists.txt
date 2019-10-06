cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
message(STATUS "CMake version: ${CMAKE_VERSION}")

project(math21)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
################################
# set lib version here

set(GENERIC_LIB_VERSION "2.1.5")
set(GENERIC_LIB_SOVERSION "5")

####################################
set(LI_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/z)
set(CMAKE_INSTALL_PREFIX ${LI_INSTALL_LOCATION})
################################
if (ANDROID)
    find_library( # Sets the name of the path variable.
            log-lib
            # Specifies the name of the NDK library that
            # you want CMake to locate.
            log)
endif ()

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
endif ()

set(MATH21_LOG ${MATH21_LOG} ${log-lib})

################ system ################
if (WIN32)
    message("IS_WIN32 ON")
    set(MATH21_FLAG_IS_WIN32 ON)
elseif (ANDROID)
    message("IS_ANDROID ON")
    set(MATH21_FLAG_IS_ANDROID ON)
elseif (APPLE)
    message("IS_APPLE ON")
    set(MATH21_FLAG_IS_APPLE ON)
elseif (UNIX)
    message("IS_LINUX ON")
    set(MATH21_FLAG_IS_LINUX ON)
else ()
    message("operating systems unknown")
    set(MATH21_FLAG_IS_LINUX ON)
endif ()
################################
# Add definitions
if (1)
    if (MATH21_FLAG_EXTERNAL)
        option(MATH21_FLAG_RELEASE "Enable Release" ${MATH21_FLAG_EXTERNAL_RELEASE})
        option(MATH21_FLAG_USE_CUDA "Use CUDA" ${MATH21_FLAG_EXTERNAL_USE_CUDA})

        #TEST
        option(MATH21_FLAG_GENERIC_STYLE "CMAKE CUDA OLD STYLE" ON)
    else ()
        option(MATH21_FLAG_RELEASE "Enable Release" ON)
        option(MATH21_FLAG_USE_CUDA "Use CUDA" ON)

        # working only when this is main project
        option(MATH21_FLAG_GENERIC_STYLE "CMAKE CUDA OLD STYLE" ON)
    endif ()

    if (WIN32)
    else ()
        option(MATH21_FLAG_USE_OPENMP "OPENMP" ON)
    endif ()

    # test openmp
    if (MATH21_FLAG_USE_CUDA)
        option(MATH21_FLAG_USE_OPENMP "OPENMP" ON)
    endif ()
endif ()

################ message ################
if (${MATH21_FLAG_USE_CUDA})
    message(STATUS "MATH21_FLAG_USE_CUDA: ${MATH21_FLAG_USE_CUDA}")
else ()
    message(STATUS "MATH21_FLAG_USE_CUDA: OFF")
endif ()
if (${MATH21_FLAG_GENERIC_STYLE})
    message(STATUS "MATH21_FLAG_GENERIC_STYLE: ${MATH21_FLAG_GENERIC_STYLE}")
else ()
    message(STATUS "MATH21_FLAG_GENERIC_STYLE: OFF")
endif ()
if (${MATH21_FLAG_USE_OPENMP})
    message(STATUS "MATH21_FLAG_USE_OPENMP: ${MATH21_FLAG_USE_OPENMP}")
else ()
    message(STATUS "MATH21_FLAG_USE_OPENMP: OFF")
endif ()

################ check ################

if (MATH21_FLAG_USE_CUDA)
    if (MATH21_FLAG_GENERIC_STYLE)
        find_package(CUDA QUIET)
        if (CUDA_FOUND)
        else ()
            set(MATH21_FLAG_USE_CUDA OFF)
        endif ()
    else ()
        check_language(CUDA)
        if (CMAKE_CUDA_COMPILER)
            enable_language(CUDA)
        else ()
            set(MATH21_FLAG_USE_CUDA OFF)
        endif ()
    endif ()
endif ()

if (${MATH21_FLAG_USE_OPENMP})
    find_package(OpenMP)
    if (OpenMP_CXX_FOUND)
    else ()
        set(MATH21_FLAG_USE_OPENMP OFF)
        message(STATUS "OpenMP not found, MATH21_FLAG_USE_OPENMP: OFF")
    endif ()
endif ()
################################
#if (MATH21_FLAG_USE_OPENMP)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#endif ()
################ configure file ################

if (${MATH21_FLAG_EXTERNAL})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/math21_user_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/math21_external_user_config_generated.h @ONLY)
else ()
    add_definitions("-DMATH21_FLAG_NOT_EXTERNAL")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/math21_user_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/math21_user_config_generated.h @ONLY)
endif ()

################################

include_directories(..)
include_directories(includes)
set(module_name math21)
set(module_test_name math21_test)

if (MATH21_FLAG_USE_CUDA)
    FILE(GLOB_RECURSE sourcefiles "3rdparty/*.c" "3rdparty/*.cc" "3rdparty/*.cu" "src/*.c" "src/*.cc" "src/*.cu")
else ()
    FILE(GLOB_RECURSE sourcefiles "3rdparty/*.c" "3rdparty/*.cc" "src/*.c" "src/*.cc")
endif ()

if (ANDROID)
    add_library(${module_name} SHARED ${sourcefiles})
else ()
    if (MATH21_FLAG_USE_CUDA)
        if (MATH21_FLAG_GENERIC_STYLE)
            cuda_add_library(${module_name} STATIC ${sourcefiles})
        else ()
            add_library(${module_name} STATIC ${sourcefiles})
        endif ()
    else ()
        add_library(${module_name} STATIC ${sourcefiles})
    endif ()
endif ()

if (${MATH21_FLAG_USE_OPENMP})
    #    target_link_libraries(${module_name} PUBLIC OpenMP::OpenMP_CXX ${MATH21_LOG})
    target_link_libraries(${module_name} OpenMP::OpenMP_CXX ${MATH21_LOG})
else ()
    target_link_libraries(${module_name} ${MATH21_LOG})
endif ()

if (ANDROID)
else ()
    if (MATH21_FLAG_USE_CUDA)
        FILE(GLOB_RECURSE SOURCE_FILES "test/*.c" "test/*.cc" "test/*.cu")
    else ()
        FILE(GLOB_RECURSE SOURCE_FILES "test/*.c" "test/*.cc")
    endif ()

    add_executable(${module_test_name} ${SOURCE_FILES})
    target_link_libraries(${module_test_name} math21)
endif ()

set_target_properties(${module_name} PROPERTIES
        COMPILE_DEFINITIONS "MATH21_EXPORT"
        VERSION "${GENERIC_LIB_VERSION}"
        SOVERSION "${GENERIC_LIB_SOVERSION}")

################ install ################

install(TARGETS ${module_name}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

if (ANDROID)
else ()
    install(TARGETS ${module_test_name}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static)
endif ()

################ ctest ################


enable_testing()
add_subdirectory(src/numbers)
add_subdirectory(unit_test/numbers)