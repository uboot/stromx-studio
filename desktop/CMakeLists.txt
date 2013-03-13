if(UNIX)
    install (
        FILES "${PROJECT_SOURCE_DIR}/desktop/stromx-studio.desktop"
        DESTINATION "share/applications"
    )
    install (
        FILES "${PROJECT_SOURCE_DIR}/desktop/stromx-studio.png"
        DESTINATION "share/icons/hicolor/48x48"
    )
endif(UNIX)