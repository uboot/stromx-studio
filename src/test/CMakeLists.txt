project(stromx_test)

add_library(stromx_test SHARED 
    TestOperator.cpp
    Test.cpp
)

target_link_libraries(stromx_test
    ${stromx_CORE_LIB}
)