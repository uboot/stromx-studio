add_subdirectory(test)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/Config.h.in"
  "${PROJECT_SOURCE_DIR}/src/Config.h"
)

set(stromxstudio_SOURCES
    AddConnectionCmd.cpp
    AddThreadCmd.cpp
    AddOperatorCmd.cpp
    ConnectionItem.cpp
    ConnectorItem.cpp
    ConnectionModel.cpp
    DeinitializeOperatorCmd.cpp
    InitializeOperatorCmd.cpp
    InputData.cpp
    InputModel.cpp
    InsertInputCmd.cpp
    InsertObserverCmd.cpp
    JoinStreamTask.cpp
    main.cpp
    MainWindow.cpp
    MoveOperatorCmd.cpp
    ObserverEditor.cpp
    ObserverModel.cpp
    ObserverTreeModel.cpp
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
    RemoveOperatorCmd.cpp
    RemoveThreadCmd.cpp
    RenameThreadCmd.cpp
    StreamEditor.cpp
    StreamEditorScene.cpp
    StreamModel.cpp
    ThreadEditor.cpp
    ThreadListModel.cpp
    ThreadModel.cpp
)

set(stromxstudio_HEADERS
    ConnectionModel.h
    InputData.h
    InputModel.h
    MainWindow.h
    ObserverEditor.h
    ObserverModel.h
    ObserverTreeModel.h
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
)

#set(stromxgui_FORMS mainwindow.ui)
set(stromxstudio_RESOURCES stromx-studio.qrc)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

qt4_wrap_cpp(stromxstudio_HEADERS_MOC ${stromxstudio_HEADERS})
#qt4_wrap_ui(stromxstudio_FORMS_HEADERS ${stromxstudio_FORMS})
qt4_add_resources(stromxstudio_RESOURCES_RCC ${stromxstudio_RESOURCES})

add_executable(stromx-studio 
    ${stromxstudio_SOURCES} 
    ${stromxstudio_HEADERS_MOC} 
#    ${stromxstudio_FORMS_HEADERS} 
    ${stromxstudio_RESOURCES_RCC}
)

target_link_libraries(stromx-studio
    ${QT_LIBRARIES}
    ${stromx_CORE_LIB}
)
