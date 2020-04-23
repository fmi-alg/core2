cmake_minimum_required(VERSION 3.9)
project(tests)

MACRO(ADD_TEST_TARGET_SINGLE _name)
	add_executable("${PROJECT_NAME}_${_name}" "${_name}.cpp")
	target_link_libraries("${PROJECT_NAME}_${_name}" core2)
	add_test(NAME "${PROJECT_NAME}_${_name}" COMMAND "${PROJECT_NAME}_${_name}")
ENDMACRO(ADD_TEST_TARGET_SINGLE)

#containers
ADD_TEST_TARGET_SINGLE(ratss)

