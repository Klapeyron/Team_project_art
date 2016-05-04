include_directories( ${CMAKE_SOURCE_DIR}/include )

find_package( OpenCV REQUIRED )

add_library ( libRummyGin
              ArtificialIntelligence.cpp
              Card.cpp
	      PrintScreenMaker.cpp
	      TableSubject.cpp
	      ImageAnalyzer.cpp
	      AreaOfInterestCutter.cpp
	      DetectionSystem.cpp
	      Position.cpp
	      ImageTemplates.cpp
	      GameControl.cpp
	    )

add_executable ( ${PROJ_NAME} main.cpp )
add_executable ( detectionTest testBinary.cpp )

target_link_libraries( ${PROJ_NAME} libRummyGin ${OpenCV_LIBS} -pthread -lX11 )
target_link_libraries( detectionTest libRummyGin ${OpenCV_LIBS} -pthread -lX11 )

set_target_properties( ${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )
set_target_properties( detectionTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )
