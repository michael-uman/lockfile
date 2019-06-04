project(serialutils)

add_library(${PROJECT_NAME} src/lockfile.c
                            include/lockfile.h
                            src/serialport.c
                            include/serialport.h
                            src/procutils.c
                            include/procutils.h)

add_library(sub::lib1 ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
