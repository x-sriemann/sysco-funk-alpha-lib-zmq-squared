message("Entering library source at: ${SRC_INCLUDES}")

file(GLOB_RECURSE HDR_FILES ${SRC_INCLUDES}/*.h)
file(GLOB_RECURSE SRC_FILES ${SRC_INCLUDES}/*.cpp)

message( "list files found")
foreach(HDR IN ITEMS ${HDR_FILES})
  message( "--- File: ${HDR} -- found")
endforeach()

add_library( ${PROJECT_NAME} STATIC
        ${SRC_FILES}
        ${HDR_FILES}
        )

#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${HDR_FILES})

target_link_libraries(${PROJECT_NAME}
        czmq
        )