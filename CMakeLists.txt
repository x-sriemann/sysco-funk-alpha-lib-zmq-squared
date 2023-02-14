cmake_minimum_required(VERSION 3.10)
project(communication LANGUAGES CXX VERSION 1.0 )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_INCLUDES  ${CMAKE_CURRENT_LIST_DIR}/src  CACHE STRING "Global includes")
set(MSGPACK_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/3rdParty/cppack/msgpack/include/msgpack CACHE STRING "MSGPACK include")

add_subdirectory(src)
add_subdirectory(test)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )