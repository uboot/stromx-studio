add_subdirectory(test)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/Config.h.in"
  "${PROJECT_SOURCE_DIR}/src/Config.h"
)

set(stromxstudio_SOURCES
    cmd/AddConnectionCmd.cpp
    cmd/AddThreadCmd.cpp
    cmd/AddOperatorCmd.cpp
    cmd/DeinitializeOperatorCmd.cpp
    cmd/InitializeOperatorCmd.cpp
    cmd/InsertInputCmd.cpp
    cmd/InsertObserverCmd.cpp
    cmd/MoveInputCmd.cpp
    cmd/MoveOperatorCmd.cpp
    cmd/RemoveConnectionCmd.cpp
    cmd/RemoveInputCmd.cpp
    cmd/RemoveObserverCmd.cpp
    cmd/RenameObserverCmd.cpp
    cmd/RemoveOperatorCmd.cpp
    cmd/RenameOperatorCmd.cpp
    cmd/RemoveThreadCmd.cpp
    cmd/RenameThreadCmd.cpp
    cmd/SetParameterCmd.cpp
    cmd/SetInputActiveCmd.cpp
    cmd/SetInputColorCmd.cpp
    cmd/SetInputVisualizationCmd.cpp
    cmd/SetThreadCmd.cpp
    cmd/SetThreadColorCmd.cpp
    item/ArrowItem.cpp
    item/ConnectionItem.cpp
    item/ConnectorItem.cpp
    item/OperatorItem.cpp
    model/ConnectionModel.cpp
    model/ErrorListModel.cpp
    model/InputModel.cpp
    model/ObserverModel.cpp
    model/ObserverTreeModel.cpp
    model/OperatorLibraryModel.cpp
    model/OperatorModel.cpp
    model/PropertyModel.cpp
    model/SelectionModel.cpp
    model/ThreadListModel.cpp
    model/ThreadModel.cpp
    model/StreamModel.cpp
    widget/DataVisualizer.cpp
    widget/DataVisualizerUtilities.cpp
    widget/DocumentationWindow.cpp
    widget/ErrorListView.cpp
    widget/GraphicsView.cpp
    widget/MainWindow.cpp
    widget/ObserverTreeView.cpp
    widget/ObserverView.cpp
    widget/ObserverWindow.cpp
    widget/OperatorLibraryView.cpp
    widget/PropertyView.cpp
    widget/SettingsDialog.cpp
    widget/StreamEditor.cpp
    widget/ThreadListView.cpp
    ChooseImageButton.cpp
    Common.cpp
    ConnectorDataEvent.cpp
    ConnectorOccupyEvent.cpp
    ConnectorObserver.cpp
    DataConverter.cpp
    DataManager.cpp
    ErrorData.cpp
    ExceptionObserver.cpp
    GetParameterTask.cpp
    Image.cpp
    InputData.cpp
    ItemDelegate.cpp
    LimitUndoStack.cpp
    main.cpp
    OperatorData.cpp
    ParameterServer.cpp
    SetParameterTask.cpp
    StreamEditorScene.cpp
    Task.cpp
    TriggerButton.cpp
    UndoStackAction.cpp
)

set(stromxstudio_HEADERS
    item/ArrowItem.h
    item/ConnectionItem.h
    item/OperatorItem.h
    model/ConnectionModel.h
    model/ErrorListModel.h
    model/InputModel.h
    model/ObserverModel.h
    model/ObserverTreeModel.h
    model/OperatorLibraryModel.h
    model/OperatorModel.h
    model/PropertyModel.h
    model/ThreadModel.h
    model/ThreadListModel.h
    model/SelectionModel.h
    model/StreamModel.h
    widget/DocumentationWindow.h
    widget/ErrorListView.h
    widget/GraphicsView.h
    widget/MainWindow.h
    widget/ObserverTreeView.h
    widget/ObserverView.h
    widget/ObserverWindow.h
    widget/OperatorLibraryView.h
    widget/PropertyView.h
    widget/SettingsDialog.h
    widget/StreamEditor.h 
    widget/ThreadListView.h
    ChooseImageButton.h
    DataManager.h
    GetParameterTask.h
    InputData.h
    ItemDelegate.h
    LimitUndoStack.h
    OperatorData.h
    ParameterServer.h
    SetParameterTask.h
    StreamEditorScene.h
    Task.h
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
