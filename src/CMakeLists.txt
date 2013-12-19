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
    cmd/SetStreamSettings.cpp
    cmd/SetThreadCmd.cpp
    cmd/SetThreadColorCmd.cpp
    cmd/SetVisualizationStateCmd.cpp
    event/ConnectorDataEvent.cpp
    event/ConnectorOccupyEvent.cpp
    data/ErrorData.cpp
    data/InputData.cpp
    data/OperatorData.cpp
    delegate/ChooseImageButton.cpp
    delegate/EditMatrixButton.cpp
    delegate/InputDelegate.cpp
    delegate/InputEditWidget.cpp
    delegate/ItemDelegate.cpp
    delegate/TriggerButton.cpp
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
    task/GetParameterTask.cpp
    task/SetParameterTask.cpp
    task/Task.cpp
    visualization/ColorChooser.cpp
    visualization/Histogram.cpp
    visualization/ImageVisualization.cpp
    visualization/DefaultVisualization.cpp
    visualization/LineSegments.cpp
    visualization/Points.cpp
    visualization/VisualizationRegistry.cpp
    visualization/VisualizationState.cpp
    widget/DataVisualizer.cpp
    widget/DocumentationWindow.cpp
    widget/ErrorListView.cpp
    widget/GraphicsView.cpp
    widget/MainWindow.cpp
    widget/MatrixEditor.cpp
    widget/ObserverTreeView.cpp
    widget/ObserverView.cpp
    widget/ObserverWindow.cpp
    widget/OperatorLibraryView.cpp
    widget/PropertyView.cpp
    widget/SettingsDialog.cpp
    widget/StreamEditor.cpp
    widget/ThreadListView.cpp
    Common.cpp
    ConnectorObserver.cpp
    DataConverter.cpp
    DataManager.cpp
    ExceptionObserver.cpp
    Image.cpp
    LimitUndoStack.cpp
    main.cpp
    Matrix.cpp
    ObserverScheduler.cpp
    ParameterServer.cpp
    StreamEditorScene.cpp
    UndoStackAction.cpp
)

set(stromxstudio_HEADERS
    data/InputData.h
    data/OperatorData.h
    delegate/ChooseImageButton.h
    delegate/EditMatrixButton.h
    delegate/InputDelegate.h
    delegate/InputEditWidget.h
    delegate/ItemDelegate.h
    delegate/TriggerButton.h
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
    task/GetParameterTask.h
    task/SetParameterTask.h
    task/Task.h
    visualization/VisualizationWidget.h
    widget/DocumentationWindow.h
    widget/ErrorListView.h
    widget/GraphicsView.h
    widget/MainWindow.h
    widget/MatrixEditor.h
    widget/ObserverTreeView.h
    widget/ObserverView.h
    widget/ObserverWindow.h
    widget/OperatorLibraryView.h
    widget/PropertyView.h
    widget/SettingsDialog.h
    widget/StreamEditor.h 
    widget/ThreadListView.h
    DataManager.h
    LimitUndoStack.h
    ParameterServer.h
    StreamEditorScene.h
    UndoStackAction.h
)

if(WIN32)
    set(stromxstudio_SOURCES 
        ${stromxstudio_SOURCES} 
        stromx-studio.rc
    )
endif(WIN32)

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
    ${stromx_CVCORE_LIB}
    ${stromx_CVIMGPROC_LIB}
    ${stromx_CVSUPPORT_LIB}
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
