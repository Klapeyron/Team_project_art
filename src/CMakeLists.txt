include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable ( ${PROJ_NAME}
	       Card.cpp
	       main.cpp
	       )
		       
target_link_libraries( ${PROJ_NAME} )

set_target_properties( ${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build )
