include_directories( ${CMAKE_SOURCE_DIR}/include )
include_directories ( /usr/include/opencv )
find_package( OpenCV REQUIRED )

add_library( Common
	     Card.cpp
	     Position.cpp
	     TableSubject.cpp
	   )

add_library( ArtificialIntelligence
	     ArtificialIntelligence.cpp
	   )

add_library( GameControl
	     GameControl.cpp
	   )

add_library( DetectionSystem
	     AreaOfInterestCutter.cpp
	     DetectionSystem.cpp
	     ImageAnalyzer.cpp
	     ImageTemplates.cpp
	     PrintScreenMaker.cpp
	   )

add_executable ( rummyGin main.cpp )
add_executable ( detectionTest testBinary.cpp )

target_link_libraries( Common -pthread )
target_link_libraries( DetectionSystem Common ${OpenCV_LIBS} -lX11 )
target_link_libraries( GameControl Common -lX11 )

target_link_libraries( rummyGin ArtificialIntelligence Common DetectionSystem GameControl )
target_link_libraries( detectionTest Common DetectionSystem )

set_target_properties( rummyGin PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )
set_target_properties( detectionTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )
