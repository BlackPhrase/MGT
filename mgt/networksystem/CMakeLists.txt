project(mgt-networksystem)

# Build as shared library
add_library(${PROJECT_NAME} MODULE)

# Sources list
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "networksystem"
)

target_link_libraries(${PROJECT_NAME} ogs-tier1)

if(WIN32)
	# NOTE: looks like wsock32 is loaded dynamically
	# ws2_32
	target_link_libraries(${PROJECT_NAME} wsock32 winmm)
endif()