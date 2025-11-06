project(
    'kindleDev',
    'cpp',
    version: 'v1.0.0',
    default_options: ['cpp_std=c++17'],
    meson_version: '>=1.1'
)

# Define dependencies we want
gtk_dep = dependency('gtk+-2.0')

# Turn off deprecation warnings (Because Gtk2 is so old)
add_project_arguments('-Wno-deprecated-declarations', language: 'cpp')

# Check for conditional compilation
if get_option('kindle_build')
    add_project_arguments('-DKINDLE_BUILD', language: 'cpp')
endif


####################
# Project definition
####################
sources = files(
    './src/main.cpp',
    './src/actor.cpp',
    './src/dungeonCell.cpp',
    './src/dungeonGeneration.cpp',
    './src/global.cpp',
    './src/viewPort.cpp',

### C-Source files for tile graphics
    './src/data/tile_at.cpp',
    './src/data/tile_cell_selected.cpp',
    './src/data/tile_floor.cpp',
    './src/data/tile_null.cpp',
    './src/data/tile_wall_cave_dualCorners_northWestToSouthEast.cpp',
    './src/data/tile_wall_cave_dualCorners_southWestToNorthEast.cpp',
    './src/data/tile_wall_cave_facing_east.cpp',
    './src/data/tile_wall_cave_facing_north.cpp',
    './src/data/tile_wall_cave_facing_south.cpp',
    './src/data/tile_wall_cave_facing_west.cpp',
    './src/data/tile_wall_cave_innerCorner_northEast.cpp',
    './src/data/tile_wall_cave_innerCorner_northWest.cpp',
    './src/data/tile_wall_cave_innerCorner_southEast.cpp',
    './src/data/tile_wall_cave_innerCorner_southWest.cpp',
    './src/data/tile_wall_cave_outerCorner_northEast.cpp',
    './src/data/tile_wall_cave_outerCorner_northWest.cpp',
    './src/data/tile_wall_cave_outerCorner_southEast.cpp',
    './src/data/tile_wall_cave_outerCorner_southWest.cpp',
    './src/data/tile_wall_cave_standalone.cpp',
)

include_dirs = include_directories(
    './include/'
)

executable(
    'kindleDev',
    sources,
    include_directories: include_dirs,
    dependencies: [gtk_dep],
    cpp_args: ['-static-libstdc++'],
    link_args: ['-static-libstdc++']
)
