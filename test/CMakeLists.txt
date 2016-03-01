enable_testing()

find_package(GTest REQUIRED)

set( PROJ_TEST_NAME      "unitTests" )

include_directories( ${CMAKE_SOURCE_DIR}/src
		     ${CMAKE_SOURCE_DIR}/include
		     ${GTEST_INCLUDE_DIRS}
		     ${GMOCK_INCLUDE_DIRS}
		    )

add_executable( ${PROJ_TEST_NAME}
		main.cpp
)

target_link_libraries( ${PROJ_TEST_NAME}
		       ${LIBRARY_NAME}
		       ${GTEST_BOTH_LIBRARIES}
		       ${GMOCK_LIBRARIES}
		       -lcrypto
		       -lgtest
		       -lgmock
		       -pthread
)

set_target_properties( ${PROJ_TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )

add_test(${PROJ_TEST_NAME} ${PROJ_TEST_NAME})
