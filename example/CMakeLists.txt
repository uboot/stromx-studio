if(DOC_INSTALL_DIR)
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/example"
        DESTINATION "${DOC_INSTALL_DIR}/stromx-studio"
        FILES_MATCHING PATTERN "*.stromx"
    )
endif()
