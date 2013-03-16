if(DOC_INSTALL_DIR)
    install (
        FILES 
        "${PROJECT_SOURCE_DIR}/example/camera.stromx"
        "${PROJECT_SOURCE_DIR}/example/hough.stromx"
        "${PROJECT_SOURCE_DIR}/example/parallel.stromx"
        "${PROJECT_SOURCE_DIR}/example/webcamera.stromx"
        DESTINATION ${DOC_INSTALL_DIR}/stromx-studio/example
    )
elseif(WIN32)
    install (
        FILES 
        "${PROJECT_SOURCE_DIR}/example/camera.stromx"
        "${PROJECT_SOURCE_DIR}/example/hough.stromx"
        "${PROJECT_SOURCE_DIR}/example/parallel.stromx"
        "${PROJECT_SOURCE_DIR}/example/webcamera.stromx"
        DESTINATION .
    )
endif()