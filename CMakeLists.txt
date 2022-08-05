cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME SimpleEngine)

add_subdirectory(SimpleEngineEditor)
add_subdirectory(SimpleEngineCore)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SimpleEngineEditor)
