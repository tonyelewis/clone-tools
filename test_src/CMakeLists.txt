
##### BEGINNING OF AUTO-GENERATED #####
set(
	TESTSOURCES_TEST
		test/build_test.cpp
		test/clone_ptr_test.cpp
		test/is_clone_ptr_test.cpp
		test/is_unique_ptr_test.cpp
		test/take_clone_test.cpp
)
##### END OF AUTO-GENERATED #####

add_executable(
	build-test
		${TESTSOURCES_TEST}
)

target_link_libraries( build-test ${Boost_LIBRARIES} )
