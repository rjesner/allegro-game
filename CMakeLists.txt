cmake_minimum_required(VERSION 3.16)
project(Game-0.0.1 VERSION 0.0.1)
set(INCLUDE_HEADER Game)
set(CMAKE_BUILD_TYPE Debug)

set(ALLEGRO_PATH third-party/Allegro)

file(GLOB_RECURSE source_list ${PROJECT_SOURCE_DIR} src/*.cpp)

add_executable(${PROJECT_NAME} ${source_list})

target_link_options(${PROJECT_NAME} PUBLIC "-mwindows")

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/${ALLEGRO_PATH}/include
)

set(ALLEGRO_LIB_DIR ${PROJECT_SOURCE_DIR}/${ALLEGRO_PATH}/lib)

target_link_libraries(${PROJECT_NAME}
    ${ALLEGRO_LIB_DIR}/liballegro_monolith-static.a
    ${ALLEGRO_LIB_DIR}/libjpeg.a
    ${ALLEGRO_LIB_DIR}/libdumb.a
    ${ALLEGRO_LIB_DIR}/libFLAC.a
    ${ALLEGRO_LIB_DIR}/libfreetype.a
    ${ALLEGRO_LIB_DIR}/libvorbisfile.a
    ${ALLEGRO_LIB_DIR}/libvorbis.a
    ${ALLEGRO_LIB_DIR}/libogg.a
    ${ALLEGRO_LIB_DIR}/libphysfs.a
    ${ALLEGRO_LIB_DIR}/libpng16.a
    ${ALLEGRO_LIB_DIR}/libzlibstatic.a
    dsound
    gdiplus
    uuid
    kernel32
    winmm
    psapi
    opengl32
    glu32
    user32
    comdlg32
    gdi32
    shell32
    ole32
    advapi32
    ws2_32
    shlwapi
    ${ALLEGRO_LIB_DIR}/libwebp.a
)

set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/build")
