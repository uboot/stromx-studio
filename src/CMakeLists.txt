add_subdirectory(test)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/Config.h.in"
  "${PROJECT_SOURCE_DIR}/src/Config.h"
)

set(stromxstudio_SOURCES
    cmd/AddConnectionCmd.cpp
    cmd/AddThreadCmd.cpp
    cmd/AddOperatorCmd.cpp
    ArrowItem.cpp
    ChooseImageButton.cpp
    Common.cpp
    ConnectionItem.cpp
    ConnectorItem.cpp
    ConnectorDataEvent.cpp
    ConnectorOccupyEvent.cpp
    ConnectorObserver.cpp
    ConnectionModel.cpp
    DataConverter.cpp
    DataManager.cpp
    DataVisualizer.cpp
    DataVisualizerUtilities.cpp
    cmd/DeinitializeOperatorCmd.cpp
    DocumentationWindow.cpp
    ErrorData.cpp
    ErrorListModel.cpp
    ErrorListView.cpp
    ExceptionObserver.cpp
    GetParameterTask.cpp
    GraphicsView.cpp
    Image.cpp
    cmd/InitializeOperatorCmd.cpp
    InputData.cpp
    InputModel.cpp
    cmd/InsertInputCmd.cpp
    cmd/InsertObserverCmd.cpp
    ItemDelegate.cpp
    LimitUndoStack.cpp
    main.cpp
    MainWindow.cpp
    cmd/MoveInputCmd.cpp
    cmd/MoveOperatorCmd.cpp
    ObserverModel.cpp
    ObserverTreeModel.cpp
    ObserverTreeView.cpp
    ObserverView.cpp
    ObserverWindow.cpp
    OperatorItem.cpp
    OperatorData.cpp
    OperatorLibraryView.cpp
    OperatorLibraryModel.cpp
    OperatorModel.cpp
    ParameterServer.cpp
    PropertyModel.cpp
    PropertyView.cpp
    cmd/RemoveConnectionCmd.cpp
    cmd/RemoveInputCmd.cpp
    cmd/RemoveObserverCmd.cpp
    cmd/RenameObserverCmd.cpp
    cmd/RemoveOperatorCmd.cpp
    cmd/RenameOperatorCmd.cpp
    cmd/RemoveThreadCmd.cpp
    cmd/RenameThreadCmd.cpp
    SelectionModel.cpp
    cmd/SetParameterCmd.cpp
    SetParameterTask.cpp
    cmd/SetInputActiveCmd.cpp
    cmd/SetInputColorCmd.cpp
    cmd/SetInputVisualizationCmd.cpp
    cmd/SetThreadCmd.cpp
    cmd/SetThreadColorCmd.cpp
    SettingsDialog.cpp
    StreamEditor.cpp
    StreamEditorScene.cpp
    StreamModel.cpp
    Task.cpp
    ThreadListModel.cpp
    ThreadListView.cpp
    ThreadModel.cpp
    TriggerButton.cpp
    UndoStackAction.cpp
)

set(stromxstudio_HEADERS
    ArrowItem.h
    ChooseImageButton.h
    ConnectionItem.h
    ConnectionModel.h
    DataManager.h
    DocumentationWindow.h
    ErrorListModel.h
    ErrorListView.h
    GetParameterTask.h
    GraphicsView.h
    InputData.h
    InputModel.h
    ItemDelegate.h
    LimitUndoStack.h
    MainWindow.h
    ObserverModel.h
    ObserverTreeModel.h
    ObserverTreeView.h
    ObserverView.h
    ObserverWindow.h
    OperatorItem.h
    OperatorData.h
    OperatorLibraryView.h
    OperatorLibraryModel.h
    OperatorModel.h
    ParameterServer.h
    PropertyModel.h
    PropertyView.h
    SelectionModel.h
    SetParameterTask.h
    SettingsDialog.h
    StreamEditor.h 
    StreamEditorScene.h
    StreamModel.h
    Task.h
    ThreadListModel.h
    ThreadListView.h
    ThreadModel.h
    TriggerButton.h
    UndoStackAction.h
)

#set(stromxgui_FORMS mainwindow.ui)
set(stromxstudio_RESOURCES stromx-studio.qrc)

if(WIN32)
    set(QT_USE_QTMAIN TRUE)
endif(WIN32)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

qt4_wrap_cpp(stromxstudio_HEADERS_MOC ${stromxstudio_HEADERS})
#qt4_wrap_ui(stromxstudio_FORMS_HEADERS ${stromxstudio_FORMS})
qt4_add_resources(stromxstudio_RESOURCES_RCC ${stromxstudio_RESOURCES})

add_executable(stromx-studio WIN32
    ${stromxstudio_SOURCES} 
    ${stromxstudio_HEADERS_MOC} 
#    ${stromxstudio_FORMS_HEADERS} 
    ${stromxstudio_RESOURCES_RCC}
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(stromx-studio
    ${QT_LIBRARIES}
    ${CMAKE_DL_LIBS}
    ${stromx_RUNTIME_LIB}
    ${stromx_EXAMPLE_LIB}
)

if(WIN32)
    install (TARGETS stromx-studio 
        RUNTIME DESTINATION .
    )
else(WIN32)
    install (TARGETS stromx-studio 
        RUNTIME DESTINATION bin
    )
endif(WIN32)
