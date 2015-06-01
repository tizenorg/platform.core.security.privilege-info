CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(PACKAGE_NAME tc-privilege-info)
PROJECT(${PACKAGE_NAME})

SET(TC1_NAME tc-privilege-info)
SET(TC1_SRCS ${PROJECT_SOURCE_DIR}/tc_privilege_info.c)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/include)

SET(requires glib-2.0 dlog)
INCLUDE(FindPkgConfig)
pkg_check_modules(${PACKAGE_NAME} REQUIRED ${requires})
FOREACH(flag ${${PACKAGE_NAME}_CFLAGS})
    SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)


SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -fPIC -Wall -fvisibility=hidden -D_WITH_SYSTEMD_")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} ${PACKAGE_NAME_CFLAGS}")
                                                                                                                                                
ADD_EXECUTABLE(${TC1_NAME} ${TC1_SRCS})
TARGET_LINK_LIBRARIES(${TC1_NAME} "privilege-info")

INSTALL(TARGETS ${TC1_NAME} DESTINATION /usr/bin)