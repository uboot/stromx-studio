add_subdirectory(test)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/Config.h.in"
  "${PROJECT_SOURCE_DIR}/src/Config.h"
)

set(stromxstudio_SOURCES
    AddConnectionCmd.cpp
    AddThreadCmd.cpp
    AddOperatorCmd.cpp
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
    DeinitializeOperatorCmd.cpp
    DocumentationWindow.cpp
    ErrorData.cpp
    ErrorListModel.cpp
    ErrorListView.cpp
    ExceptionObserver.cpp
    GraphicsView.cpp
    Image.cpp
    InitializeOperatorCmd.cpp
    InputData.cpp
    InputModel.cpp
    InsertInputCmd.cpp
    InsertObserverCmd.cpp
    ItemDelegate.cpp
    LimitUndoStack.cpp
    main.cpp
    MainWindow.cpp
    MoveInputCmd.cpp
    MoveOperatorCmd.cpp
    ObserverModel.cpp
    ObserverTreeModel.cpp
    ObserverTreeView.cpp
    ObserverView.cpp
    ObserverWindow.cpp
    ObtainReadAccessTask.cpp
    OperatorItem.cpp
    OperatorData.cpp
    OperatorLibraryView.cpp
    OperatorLibraryModel.cpp
    OperatorModel.cpp
    ParameterServer.cpp
    PropertyModel.cpp
    PropertyView.cpp
    RemoveConnectionCmd.cpp
    RemoveInputCmd.cpp
    RemoveObserverCmd.cpp
    RenameObserverCmd.cpp
    RemoveOperatorCmd.cpp
    RenameOperatorCmd.cpp
    RemoveThreadCmd.cpp
    RenameThreadCmd.cpp
    SelectionModel.cpp
    SetParameterCmd.cpp
    SetInputActiveCmd.cpp
    SetInputColorCmd.cpp
    SetInputVisualizationCmd.cpp
    SetThreadCmd.cpp
    SetThreadColorCmd.cpp
    StreamEditor.cpp
    StreamEditorScene.cpp
    StreamModel.cpp
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
    ObtainReadAccessTask.h
    OperatorItem.h
    OperatorData.h
    OperatorLibraryView.h
    OperatorLibraryModel.h
    OperatorModel.h
    ParameterServer.h
    PropertyModel.h
    PropertyView.h
    SelectionModel.h
    StreamEditor.h 
    StreamEditorScene.h
    StreamModel.h
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

target_link_libraries(stromx-studio
    ${QT_LIBRARIES}
    ${CMAKE_DL_LIBS}
    ${stromx_CORE_LIB}
    ${stromx_BASE_LIB}
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

