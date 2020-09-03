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
        src/GameAccess.cpp include/GameAccess.hpp
        src/GameState.cpp include/GameState.hpp

        src/Palette.cpp include/Palette.hpp
        src/TextBox.cpp include/TextBox.hpp
        src/MapCollisions.cpp include/MapCollisions.hpp
        src/HUD.cpp include/HUD.hpp
        src/Spike.cpp include/Spike.hpp src/Artifact.cpp include/Artifact.hpp src/Item.cpp include/Item.hpp

        src/entities/Player.cpp include/entities/Player.hpp
        src/entities/Enemy.cpp include/entities/Enemy.hpp
        src/entities/Mummy.cpp include/entities/Mummy.hpp
        src/entities/Wasp.cpp include/entities/Wasp.hpp

        src/transitions/PaletteShiftTransition.cpp include/transitions/PaletteShiftTransition.hpp
        src/transitions/WaveTransition.cpp include/transitions/WaveTransition.hpp

        src/states/LevelState.cpp include/states/LevelState.hpp
        src/states/TitleScreenState.cpp include/states/TitleScreenState.hpp
        src/states/MuseumLevelState.cpp include/states/MuseumLevelState.hpp
        src/states/WarehouseLevelState.cpp include/states/WarehouseLevelState.hpp
)

target_include_directories(GB_OLC_Jam PRIVATE include)
target_link_libraries(GB_OLC_Jam sfml-graphics sfml-audio)