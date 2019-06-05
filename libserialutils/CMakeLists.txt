project(serialutils)

add_library(${PROJECT_NAME} src/siot_lockfile.c
        include/siot_lockfile.h
        src/siot_serialport.c
        include/siot_serialport.h
        src/siot_procutils.c
        include/siot_procutils.h)

add_library(sub::lib1 ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
