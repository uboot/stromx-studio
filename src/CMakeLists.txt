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
    ConnectionItem.cpp
    ConnectorItem.cpp
    ConnectorDataEvent.cpp
    ConnectorOccupyEvent.cpp
    ConnectorObserver.cpp
    ConnectionModel.cpp
    DataConverter.cpp
    DataManager.cpp
    DataVisualizer.cpp
    DeinitializeOperatorCmd.cpp
    Image.cpp
    InitializeOperatorCmd.cpp
    InputData.cpp
    InputModel.cpp
    InsertInputCmd.cpp
    InsertObserverCmd.cpp
    ItemDelegate.cpp
    JoinStreamTask.cpp
    LimitUndoStack.cpp
    main.cpp
    MainWindow.cpp
    MoveInputCmd.cpp
    MoveOperatorCmd.cpp
    ObserverEditor.cpp
    ObserverModel.cpp
    ObserverTreeModel.cpp
    ObserverWindow.cpp
    OperatorItem.cpp
    OperatorData.cpp
    OperatorLibrary.cpp
    OperatorLibraryList.cpp
    OperatorLibraryModel.cpp
    OperatorModel.cpp
    PropertyEditor.cpp
    RemoveConnectionCmd.cpp
    RemoveInputCmd.cpp
    RemoveObserverCmd.cpp
    RenameObserverCmd.cpp
    RemoveOperatorCmd.cpp
    RenameOperatorCmd.cpp
    RemoveThreadCmd.cpp
    RenameThreadCmd.cpp
    SetParameterCmd.cpp
    SetInputColorCmd.cpp
    SetThreadCmd.cpp
    SetThreadColorCmd.cpp
    StreamEditor.cpp
    StreamEditorScene.cpp
    StreamModel.cpp
    ThreadEditor.cpp
    ThreadListModel.cpp
    ThreadModel.cpp
    UndoStackAction.cpp
)

set(stromxstudio_HEADERS
    ArrowItem.h
    ConnectionItem.h
    ConnectionModel.h
    DataManager.h
    InputData.h
    InputModel.h
    ItemDelegate.h
    LimitUndoStack.h
    MainWindow.h
    ObserverEditor.h
    ObserverModel.h
    ObserverTreeModel.h
    ObserverWindow.h
    OperatorItem.h
    OperatorData.h
    OperatorLibrary.h
    OperatorLibraryList.h
    OperatorLibraryModel.h
    OperatorModel.h
    PropertyEditor.h
    StreamEditor.h 
    StreamEditorScene.h
    StreamModel.h
    ThreadEditor.h
    ThreadListModel.h
    ThreadModel.h
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

