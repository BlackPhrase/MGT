aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES_NETWORK)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES_NETWORK}
)