CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libuseful)

FIND_PROGRAM(DiffFound diff)
IF(DiffFound STREQUAL "DiffFound-NOTFOUND")
    MESSAGE(FATAL_ERROR "The diff tool is not found on your system.  This is required for testing!")
ENDIF(DiffFound STREQUAL "DiffFound-NOTFOUND")

FIND_PATH(LIBUSEFUL_INCLUDES 
			test_utils.h
			hashtable.h
			list.h
			optin.h
			platform.h
			stringbuilder.h
			${CMAKE_CURRENT_SOURCE_DIR}/include
)

INCLUDE_DIRECTORIES(${LIBUSEFUL_INCLUDES})

SET(useful_LIB_SRCS
    platform.c
	hashtable.c
    list.c
    stringbuilder.c
    optin.c
    include/test_utils.h
	include/platform.h
	include/hashtable.h
    include/list.h
    include/stringbuilder.h
	include/optin.h
)

ADD_LIBRARY(useful ${useful_LIB_SRCS})
