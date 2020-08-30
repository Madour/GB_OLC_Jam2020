cmake_minimum_required(VERSION 3.12)

project(GB_OLC_Jam)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

if (WIN32)
    set(SFML_ROOT C:/SFML)    # specify here the install location of SFML
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML COMPONENTS graphics audio PATHS ${SFML_ROOT}/lib/cmake/ )
else()
    find_package(SFML COMPONENTS graphics audio )
endif()

# the following variables can be set to FALSE to exclude the associated module
set(NASNAS_BUILD_RESLIB         TRUE)
set(NASNAS_BUILD_ECS            TRUE)
set(NASNAS_BUILD_TILEMAPPING    TRUE)
set(NASNAS_BUILD_UI             TRUE)

include(cmake/NasNas.cmake)

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(GB_OLC_Jam
        NasNas.h ${NasNas_Src} ${NasNas_Inc}
        main.cpp
        src/Game.cpp include/Game.hpp
        src/Palette.cpp include/Palette.hpp
        src/PaletteShiftTransition.cpp include/PaletteShiftTransition.hpp
        src/Player.cpp include/Player.hpp
        src/TextBox.cpp include/TextBox.hpp
        src/MapCollisions.cpp include/MapCollisions.hpp

        src/GameState.cpp include/GameState.hpp
        src/states/LevelState.cpp include/states/LevelState.hpp
        src/states/TitleScreenState.cpp include/states/TitleScreenState.hpp src/states/MuseumLevelState.cpp include/states/MuseumLevelState.hpp)

# Demo game target
target_include_directories(GB_OLC_Jam PRIVATE include)
target_link_libraries(GB_OLC_Jam sfml-graphics sfml-audio)