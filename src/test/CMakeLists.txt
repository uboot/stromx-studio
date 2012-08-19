project(stromx_studio_test)

add_test(NAME stromx_studio_test COMMAND stromx_studio_test)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna_bw.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(stromxstudiotest_HEADERS
    DataConverterTest.h
    StreamModelTest.h
    ImageTest.h
)

set(stromxstudiotest_SOURCES
    main.cpp
    DataConverterTest.cpp
    ImageTest.cpp
    StreamModelTest.cpp
    ../DataConverter.cpp
    ../Image.cpp
)

set(QT_USE_QTTEST TRUE) 

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
qt4_wrap_cpp(stromxstudiotest_HEADERS_MOC ${stromxstudiotest_HEADERS})

add_executable(stromx_studio_test
    ${stromxstudiotest_SOURCES} 
    ${stromxstudiotest_HEADERS_MOC} 
)

target_link_libraries(stromx_studio_test
    ${QT_LIBRARIES}
    ${stromx_CORE_LIB}
)

add_library(stromx_test SHARED 
    DeadlockOperator.cpp
    ExceptionOperator.cpp
    ParameterOperator.cpp
    Test.cpp
)

target_link_libraries(stromx_test
    ${stromx_CORE_LIB}
)