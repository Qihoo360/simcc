
file(GLOB simcc_SRCS *.cc */*.cc *.h */*.h)
file(GLOB simcc_PUBLIC_HEADERS *.h )
file(GLOB simcc_JSON_PUBLIC_HEADERS json/*.h)
file(GLOB simcc_MISC_PUBLIC_HEADERS misc/*.h)

message(STATUS "simcc_SRCS : " ${simcc_SRCS})

set (MACOSX_RPATH ${LIBRARY_OUTPUT_PATH})

include_directories(${PROJECT_SOURCE_DIR})

add_library(simcc_static STATIC ${simcc_SRCS})
target_link_libraries(simcc_static ${DEPENDENT_LIBRARIES})

if (UNIX)
    add_library(simcc SHARED ${simcc_SRCS})
    target_link_libraries(simcc ${DEPENDENT_LIBRARIES})

    set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
    include (utils)
    include (packages)
    set_target_properties (
        simcc PROPERTIES
        VERSION     "${PACKAGE_VERSION}"
        SOVERSION   "${PACKAGE_SOVERSION}"
    )

    install (
      TARGETS simcc simcc_static 
      EXPORT ${PACKAGE_NAME}
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib)

else (UNIX)
    install (
      TARGETS simcc_static 
      EXPORT ${PACKAGE_NAME}
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib)

endif (UNIX)

install (FILES ${simcc_PUBLIC_HEADERS} DESTINATION "include/simcc")
install (FILES ${simcc_JSON_PUBLIC_HEADERS} DESTINATION "include/simcc/json")
install (FILES ${simcc_MISC_PUBLIC_HEADERS} DESTINATION "include/simcc/misc")
