CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(PACKAGE_NAME privilege-info)
PROJECT(${PACKAGE_NAME})

SET(PRIV_INFO_SOURCES
    ${PROJECT_SOURCE_DIR}/privilege_info.c
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

SET(requires glib-2.0 dlog security-privilege-manager)
INCLUDE(FindPkgConfig)
pkg_check_modules(${PACKAGE_NAME} REQUIRED ${requires})
FOREACH(flag ${${PACKAGE_NAME}_CFLAGS})
    SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

# Compiler flags
SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -fPIC -Wall -fvisibility=hidden -D_WITH_SYSTEMD_")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} ${PACKAGE_NAME_CFLAGS}")

########################################################
# For libprivilege-info libraries(.so)
########################################################

SET(TARGET_PRIV_INFO_LIB "privilege-info")
SET(SO_VERSION 1)
SET(VERSION "${SO_VERSION}.0.0")

ADD_LIBRARY(${TARGET_PRIV_INFO_LIB} SHARED ${PRIV_INFO_SOURCES})

SET_TARGET_PROPERTIES(${TARGET_PRIV_INFO_LIB} PROPERTIES
    SOVERSION ${SO_VERSION}
    VERSION ${VERSION}
)

TARGET_LINK_LIBRARIES(${TARGET_PRIV_INFO_LIB} ${${PACKAGE_NAME}_LDFLAGS})

#######################################################

INSTALL(TARGETS ${PACKAGE_NAME} DESTINATION ${LIBDIR})
INSTALL(FILES ${PROJECT_SOURCE_DIR}/include/privilege_information.h DESTINATION ${INCLUDEDIR})
INSTALL(FILES ${CMAKE_SOURCE_DIR}/privilege-info.pc DESTINATION ${LIBDIR}/pkgconfig)
