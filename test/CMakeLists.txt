#
# Copyright (c) 2023 Vaillant Group International GmbH. All rights reserved.
#

find_package(GTest REQUIRED)
option(TRACY_ENABLE "Enable the Profiler" ON)
option(TRACY_ON_DEMAND "" ON)

enable_testing()

set(TEST_SOURCES
        TestZMQPubSubAround.cpp
        )

add_executable(${PROJECT_NAME}-unit-test
        ${TEST_SOURCES}
)

target_include_directories(${PROJECT_NAME}-unit-test PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${SRC_INCLUDES}
        ${TRACY_INCLUDES}
        )

target_link_libraries(${PROJECT_NAME}-unit-test
        ${PROJECT_NAME}
        zmq
        gtest
        gtest_main
        gmock
        gmock_main
        TracyClient
        )

gtest_discover_tests(${PROJECT_NAME}-unit-test XML_OUTPUT_DIR ${PROJECT_NAME}-unit-test.xml)
add_test(NAME unittests COMMAND ${PROJECT_NAME}-unit-test --gtest_output="xml:./${PROJECT_NAME}-unit-test.xml")

install(TARGETS ${PROJECT_NAME}-unit-test DESTINATION bin)