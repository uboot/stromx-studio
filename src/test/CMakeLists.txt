project(stromx_studio_test)
  
set(stromxstudiotest_SOURCES
    ../OperatorData.cpp
    ../OperatorLibraryModel.cpp
    OperatorLibraryModelTest.cpp
    main.cpp
)

set(stromxstudiotest_HEADERS
    ../OperatorData.h
    ../OperatorLibraryModel.h
)

configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_SOURCE_DIR}/Config.h"
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
    ${stromx_CORE_LIB}
    ${CPPUNIT_LIBRARY}
    ${CMAKE_DL_LIBS}
)