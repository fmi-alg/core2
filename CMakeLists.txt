cmake_minimum_required(VERSION 3.9)
project(core2)

set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

find_package(Threads)
find_package(LIBGMPXX REQUIRED)
find_package(LIBMPFR REQUIRED)


set(CORE_TWO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")

set(CORE_TWO_COMPILE_DEFINITIONS
	"CORE_TWO_LEVEL=4"
	"CORE_TWO_NO_AUTOMATIC_NAMESPACE=1"
)

set(CORE_TWO_LINK_LIBRARIES
	${LIBGMPXX_LIBRARIES}
	${LIBMPFR_LIBRARIES}
)

set(LIB_SOURCES_CPP
	src/CoreAux.cpp
	src/CoreDefs.cpp
	src/CoreIo.cpp
	src/MpfrIO.cpp
)

add_library(${PROJECT_NAME} STATIC
	${LIB_SOURCES_CPP}
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_include_directories(${PROJECT_NAME} PUBLIC ${CORE_TWO_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CORE_TWO_LINK_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${CORE_TWO_COMPILE_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PUBLIC ${CORE_TWO_COMPILE_OPTIONS})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

add_subdirectory(tests ${PROJECT_NAME}tests)

