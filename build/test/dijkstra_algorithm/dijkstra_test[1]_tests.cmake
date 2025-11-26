add_test( Dijkstra.BasicPathFinding /home/lukasz/CLionProjects/dijkstra_multithreaded/build/test/dijkstra_algorithm/dijkstra_test [==[--gtest_filter=Dijkstra.BasicPathFinding]==] --gtest_also_run_disabled_tests)
set_tests_properties( Dijkstra.BasicPathFinding PROPERTIES WORKING_DIRECTORY /home/lukasz/CLionProjects/dijkstra_multithreaded/build/test/dijkstra_algorithm SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( dijkstra_test_TESTS Dijkstra.BasicPathFinding)
