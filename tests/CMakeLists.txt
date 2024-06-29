set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
set(TESTS_SRC
	test_adjacent_flat_map.cpp
	test_flat_map.cpp
	test_flat_set.cpp
	test_pair_algorithm.cpp
	test_unordered_adjacent_flat_map.cpp
	test_unordered_flat_map.cpp
	test_unordered_flat_set.cpp
	tests.cpp
)
add_executable(run-tests ${TESTS_SRC})
target_include_directories(run-tests
	PUBLIC ${PROJECT_SOURCE_DIR}
	PUBLIC ${PROJECT_SOURCE_DIR}/googletest/googletest/include
)
target_link_libraries(run-tests
	gtest
)
