if(DOC_INSTALL_DIR)
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/example"
        DESTINATION "${DOC_INSTALL_DIR}/stromx-studio"
        FILES_MATCHING PATTERN "*.stromx"
    )
elseif(WIN32)
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/example"
        DESTINATION .
        FILES_MATCHING PATTERN "*.stromx"
    )
endif()