add_library(cells
    positions.hpp
    cell_data.hpp
    cell_stack_matrix.hpp cell_stack_matrix.cpp
    cell_types.hpp

    cell.hpp cell.cpp
    empty_cell.hpp empty_cell.cpp
    data_cell.hpp data_cell.cpp
    signal_cell.hpp signal_cell.cpp
    wire_cell.hpp wire_cell.cpp
    belt_cell.hpp belt_cell.cpp
    clock_cell.hpp clock_cell.cpp
)

target_include_directories(cells
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(cells
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui)
