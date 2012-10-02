project(stromx_studio_test)

add_test(NAME stromx_studio_test COMMAND stromx_studio_test)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/camera.stromx DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/connector.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna_bw.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(stromxstudiotest_HEADERS
    DataConverterTest.h
    ImageTest.h
    ParameterServerTest.h
    StreamModelTest.h
    ../ConnectionModel.h
    ../GetParameterTask.h
    ../InputData.h
    ../InputModel.h
    ../ObserverModel.h
    ../ObserverTreeModel.h
    ../ObtainReadAccessTask.h
    ../OperatorData.h
    ../OperatorLibraryModel.h
    ../OperatorModel.h
    ../ParameterServer.h
    ../PropertyModel.h
    ../ThreadListModel.h
    ../ThreadModel.h
    ../StreamModel.h
)

set(stromxstudiotest_SOURCES
    main.cpp
    DataConverterTest.cpp
    ImageTest.cpp
    ParameterOperator.cpp
    ParameterServerTest.cpp
    StreamModelTest.cpp
    ../AddConnectionCmd.cpp
    ../AddOperatorCmd.cpp
    ../AddThreadCmd.cpp
    ../Common.cpp
    ../ConnectionModel.cpp
    ../ConnectorObserver.cpp
    ../ConnectorDataEvent.cpp
    ../ConnectorOccupyEvent.cpp
    ../DataConverter.cpp
    ../DeinitializeOperatorCmd.cpp
    ../GetParameterTask.cpp
    ../Image.cpp
    ../InitializeOperatorCmd.cpp
    ../InputData.cpp
    ../InputModel.cpp
    ../InsertInputCmd.cpp
    ../InsertObserverCmd.cpp
    ../MoveInputCmd.cpp
    ../MoveOperatorCmd.cpp
    ../OperatorData.cpp
    ../OperatorLibraryModel.cpp
    ../ObserverModel.cpp
    ../ObserverTreeModel.cpp
    ../ObtainReadAccessTask.cpp
    ../OperatorModel.cpp
    ../ParameterServer.cpp
    ../PropertyModel.cpp
    ../RemoveInputCmd.cpp
    ../RemoveConnectionCmd.cpp
    ../RemoveOperatorCmd.cpp
    ../RemoveObserverCmd.cpp
    ../RemoveThreadCmd.cpp
    ../RenameObserverCmd.cpp
    ../RenameOperatorCmd.cpp
    ../RenameThreadCmd.cpp
    ../SetInputActiveCmd.cpp
    ../SetInputColorCmd.cpp
    ../SetInputVisualizationCmd.cpp
    ../SetParameterCmd.cpp
    ../SetThreadCmd.cpp
    ../SetThreadColorCmd.cpp
    ../StreamModel.cpp
    ../ThreadListModel.cpp
    ../ThreadModel.cpp
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
    ${stromx_BASE_LIB}
)

add_library(stromx_test SHARED 
    DeadlockOperator.cpp
    ExceptionOperator.cpp
    ParameterOperator.cpp
    RandomDataOperator.cpp
    Test.cpp
)

target_link_libraries(stromx_test
    ${stromx_CORE_LIB}
    ${stromx_BASE_LIB}
)