message("cpp_sgr tests enabled")

include_directories(
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

link_libraries(cpp_sgr)

add_executable(test_simple
  test_simple.cpp)

add_test(simple
  test_simple)

add_executable(test_multi
  test_multi.cpp)

add_test(multi
  test_multi)

add_executable(test_reset
	test_reset.cpp)

add_test(reset
	test_reset)
