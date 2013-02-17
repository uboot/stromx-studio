project(stromx_studio_test)

add_test(NAME stromx_studio_test COMMAND stromx_studio_test)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/camera.stromx DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/extra_parameter.stromx DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
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
    ../OperatorData.h
    ../OperatorLibraryModel.h
    ../OperatorModel.h
    ../ParameterServer.h
    ../PropertyModel.h
    ../Task.h
    ../ThreadListModel.h
    ../ThreadModel.h
    ../SetParameterTask.h
    ../StreamModel.h
)

set(stromxstudiotest_SOURCES
    main.cpp
    DataConverterTest.cpp
    ImageTest.cpp
    ParameterOperator.cpp
    ParameterServerTest.cpp
    StreamModelTest.cpp
    ../cmd/AddConnectionCmd.cpp
    ../cmd/AddOperatorCmd.cpp
    ../cmd/AddThreadCmd.cpp
    ../Common.cpp
    ../ConnectionModel.cpp
    ../ConnectorObserver.cpp
    ../ConnectorDataEvent.cpp
    ../ConnectorOccupyEvent.cpp
    ../DataConverter.cpp
    ../cmd/DeinitializeOperatorCmd.cpp
    ../ErrorData.cpp
    ../ExceptionObserver.cpp
    ../GetParameterTask.cpp
    ../Image.cpp
    ../cmd/InitializeOperatorCmd.cpp
    ../InputData.cpp
    ../InputModel.cpp
    ../cmd/InsertInputCmd.cpp
    ../cmd/InsertObserverCmd.cpp
    ../cmd/MoveInputCmd.cpp
    ../cmd/MoveOperatorCmd.cpp
    ../OperatorData.cpp
    ../OperatorLibraryModel.cpp
    ../ObserverModel.cpp
    ../ObserverTreeModel.cpp
    ../OperatorModel.cpp
    ../ParameterServer.cpp
    ../PropertyModel.cpp
    ../cmd/RemoveInputCmd.cpp
    ../cmd/RemoveConnectionCmd.cpp
    ../cmd/RemoveOperatorCmd.cpp
    ../cmd/RemoveObserverCmd.cpp
    ../cmd/RemoveThreadCmd.cpp
    ../cmd/RenameObserverCmd.cpp
    ../cmd/RenameOperatorCmd.cpp
    ../cmd/RenameThreadCmd.cpp
    ../cmd/SetInputActiveCmd.cpp
    ../cmd/SetInputColorCmd.cpp
    ../cmd/SetInputVisualizationCmd.cpp
    ../cmd/SetParameterCmd.cpp
    ../SetParameterTask.cpp
    ../cmd/SetThreadCmd.cpp
    ../cmd/SetThreadColorCmd.cpp
    ../StreamModel.cpp
    ../Task.cpp
    ../ThreadListModel.cpp
    ../ThreadModel.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
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
    ${stromx_RUNTIME_LIB}
    ${stromx_EXAMPLE_LIB}
)

add_library(stromx_test SHARED 
    DeadlockOperator.cpp
    ExceptionOperator.cpp
    ParameterOperator.cpp
    RandomDataOperator.cpp
    Test.cpp
)

target_link_libraries(stromx_test
    ${stromx_RUNTIME_LIB}
    ${stromx_EXAMPLE_LIB}
)