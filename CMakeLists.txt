cmake_minimum_required(VERSION 3.28)
project(TypeTraits)

add_executable(${PROJECT_NAME})

set(SOURCES_DIR "src")

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "${SOURCES_DIR}/*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS "${SOURCES_DIR}/*.hpp")

target_sources(${PROJECT_NAME} PRIVATE
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCES_DIR})
