set(stromxgui_SOURCES main.cpp mainwindow.cpp)
set(stromxgui_HEADERS mainwindow.h)
#set(stromxgui_FORMS mainwindow.ui)
set(stromxgui_RESOURCES stromxgui.qrc)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

qt4_wrap_cpp(stromxgui_HEADERS_MOC ${stromxgui_HEADERS})
#qt4_wrap_ui(stromxgui_FORMS_HEADERS ${stromxgui_FORMS})
qt4_add_resources(stromxgui_RESOURCES_RCC ${stromxgui_RESOURCES})

include_directories(
    ${STROMX_INCLUDE_DIR}
)

add_executable(stromx-gui 
    ${stromxgui_SOURCES} 
    ${stromxgui_HEADERS_MOC} 
#    ${stromxgui_FORMS_HEADERS} 
    ${stromxgui_RESOURCES_RCC}
)

target_link_libraries(stromx-gui
    ${QT_LIBRARIES}
    ${STROMX_LIBRARY}
)
