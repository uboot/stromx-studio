project(stromx_studio_test)
  
set(stromxstudiotest_SOURCES
    ../OperatorLibraryModel.cpp
    OperatorLibraryModelTest.cpp
    main.cpp
)

set(stromxstudiotest_HEADERS
    ../OperatorLibraryModel.h
)


include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

qt4_wrap_cpp(stromxstudiotest_HEADERS_MOC ${stromxstudiotest_HEADERS})

include_directories(
    ${STROMX_INCLUDE_DIR}
    ${CPPUNIT_INCLUDE_DIR}
)

add_executable(stromx_studio_test
    ${stromxstudiotest_SOURCES} 
    ${stromxstudiotest_HEADERS_MOC} 
)

target_link_libraries(stromx_studio_test
    ${QT_LIBRARIES}
    ${STROMX_LIBRARY}
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
)