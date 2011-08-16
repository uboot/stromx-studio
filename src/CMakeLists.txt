set(stromxstudio_SOURCES
    main.cpp
    MainWindow.cpp
    ObserverEditor.cpp
    OperatorLibrary.cpp
    OperatorLibraryList.cpp
    OperatorLibraryModel.cpp
    PropertyEditor.cpp
    StreamEditor.cpp
    StreamEditorScene.cpp
    StromxData.cpp
    ThreadEditor.cpp
)

set(stromxstudio_HEADERS
    MainWindow.h
    ObserverEditor.h
    OperatorLibrary.h
    OperatorLibraryList.h
    PropertyEditor.h
    StreamEditor.h 
    StreamEditorScene.h
    StromxData.h
    ThreadEditor.h
)

#set(stromxgui_FORMS mainwindow.ui)
set(stromxstudio_RESOURCES stromx-studio.qrc)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

qt4_wrap_cpp(stromxstudio_HEADERS_MOC ${stromxstudio_HEADERS})
#qt4_wrap_ui(stromxstudio_FORMS_HEADERS ${stromxstudio_FORMS})
qt4_add_resources(stromxstudio_RESOURCES_RCC ${stromxstudio_RESOURCES})

include_directories(
    ${STROMX_INCLUDE_DIR}
)

add_executable(stromx-studio 
    ${stromxstudio_SOURCES} 
    ${stromxstudio_HEADERS_MOC} 
#    ${stromxstudio_FORMS_HEADERS} 
    ${stromxstudio_RESOURCES_RCC}
)

target_link_libraries(stromx-studio
    ${QT_LIBRARIES}
    ${STROMX_CORE_LIBRARY}
)


if(BUILD_TESTS)
    add_subdirectory(test)
endif()