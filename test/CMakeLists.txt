enable_testing()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set( PROJ_TEST_NAME      "unitTests" )

include_directories( ${CMAKE_SOURCE_DIR}/include ${GTEST_INCLUDE_DIR})

include_directories( ${GTEST_INCLUDE_DIRS}
		     ${GMOCK_INCLUDE_DIRS}
		    )

add_executable( ${PROJ_TEST_NAME}
		CardTest.cpp
		ArtificialIntelligenceTest.cpp
		DetectionSystemTest.cpp
		ImageAnalyzerTest.cpp
		main.cpp
)

target_link_libraries(	${PROJ_TEST_NAME}
			ArtificialIntelligence
			Common
			DetectionSystem
			GameControl
)

target_link_libraries( ${PROJ_TEST_NAME}
		       ${GTEST_BOTH_LIBRARIES}
		       ${GMOCK_LIBRARIES}
		       ${CMAKE_THREAD_LIBS_INIT}
		       ${OpenCV_LIBS}
		       -lgmock
)

set_target_properties( ${PROJ_TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )

add_test( ${PROJ_TEST_NAME} ${CMAKE_SOURCE_DIR}/build/${PROJ_TEST_NAME} COMMAND ${PROJ_TEST_NAME} )
