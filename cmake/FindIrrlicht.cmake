#
# A CMake module to find Irrlicht for the project B-YEP-400 IndieStudio
#
# This CMake module is designed to work on Fedora28 and Windows10. We will not
# provide support for other distributions. We will use this module to compile
# your project during our tests.
#
# You must provide the path to this module to CMake by setting
# CMAKE_MODULE_PATH variable to its location.
#
# Once done this module will define:
#  Irrlicht_FOUND - system has Irrlicht
#  Irrlicht_INCLUDE_DIRS - the Irrlicht include directory
#  Irrlicht_LIBRARIES - Link these to use Irrlicht
#
# Author: Guillaume DEVOILLE <guillaume1.devoille@epitech.eu> (17/05/2019)
#

IF (NOT Irrlicht_INCLUDE_DIRS OR NOT Irrlicht_LIBRARIES)
    FIND_PATH(Irrlicht_INCLUDE_DIRS
            NAMES
            irrlicht/irrlicht.h
            PATHS
            /usr/include/        # Default Fedora28 system include path
            /usr/local/include/  # Default Fedora28 local include path
            ${CMAKE_MODULE_PATH}/include/ # Expected to contain the path to this file for Windows10
            ${Irrlicht_DIR}/include/ # Irrlicht root directory (if provided)
            )
    IF (MSVC)     # Windows
        SET(CMAKE_FIND_LIBRARY_PREFIXES "")
        SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
    ELSE (MSVC)   # Linux
        SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
        SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
    ENDIF (MSVC)

    FIND_LIBRARY(Irrlicht_LIBRARIES
            NAMES
            irrlicht
            PATHS
            /usr/lib64/
            /usr/lib/
            /usr/lib/x86_64-linux-gnu/
            /usr/local/lib/
            /usr/local/lib64/
            ${CMAKE_MODULE_PATH}/
            ${Irrlicht_DIR}/
            )
ENDIF (NOT Irrlicht_INCLUDE_DIRS OR NOT Irrlicht_LIBRARIES)

IF (Irrlicht_INCLUDE_DIRS AND Irrlicht_LIBRARIES)
    SET(Irrlicht_FOUND TRUE)
ELSE (Irrlicht_INCLUDE_DIRS AND Irrlicht_LIBRARIES)
    SET(Irrlicht_FOUND FALSE)
ENDIF (Irrlicht_INCLUDE_DIRS AND Irrlicht_LIBRARIES)

IF (Irrlicht_FIND_REQUIRED AND NOT Irrlicht_FOUND)
    MESSAGE(FATAL_ERROR
            "  Irrlicht not found.\n"
            "      Windows: Fill CMake variable CMAKE_MODULE_PATH to the provided directory.\n"
            "      Linux: Install Irrlicht using your package manager ($> sudo dnf install irrlicht-devel).\n"
            )
ENDIF (Irrlicht_FIND_REQUIRED AND NOT Irrlicht_FOUND)
