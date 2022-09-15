set(TS_FILES ${PROJECT_NAME}_en_gb.ts)

target_sources(
    ${PROJECT_NAME}
  PRIVATE
    ${TS_FILES}
)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
