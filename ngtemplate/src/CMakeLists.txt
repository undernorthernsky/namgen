CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ngtemplate)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(NOT LIBUSEFUL_DIR)
	SET(LIBUSEFUL_DIR ../lib/libuseful/src)
	ADD_SUBDIRECTORY(${LIBUSEFUL_DIR} "${CMAKE_CURRENT_BINARY_DIR}/libuseful")
ENDIF(NOT LIBUSEFUL_DIR)

INCLUDE_DIRECTORIES(${LIBUSEFUL_INCLUDES})

FIND_PATH(NGTEMPLATE_INCLUDES 
			ngtemplate.h
			${CMAKE_CURRENT_SOURCE_DIR}/include
)

INCLUDE_DIRECTORIES(${NGTEMPLATE_INCLUDES})


SET(ngtemplate_LIB_SRCS
	internal.h
	internal.c
	stdenv.c
	ngtemplate.c
	include/ngtemplate.h
)

ADD_LIBRARY(ngtemplate STATIC ${ngtemplate_LIB_SRCS})
TARGET_LINK_LIBRARIES(ngtemplate useful)
