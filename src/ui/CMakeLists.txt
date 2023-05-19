include_directories(${CMAKE_CURRENT_SOURCE_DIR})

qt_add_executable(cellar
    main.cpp
    mainwindow.cpp mainwindow.hpp mainwindow.ui
    draw_area.hpp draw_area.cpp
    cell_item.hpp cell_item.cpp
)

set_target_properties(cellar
  PROPERTIES
    OUTPUT_NAME receptor
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

target_link_libraries(cellar
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
    PUBLIC
        cells)
