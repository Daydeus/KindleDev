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
    './src/viewPort.cpp',

### C-Source files for tile graphics
    './src/data/tile_null.cpp',
    './src/data/tile_floor_base.cpp',
    './src/data/tile_wall_top.cpp',
    './src/data/tile_wall_right.cpp',
    './src/data/tile_wall_top_right.cpp',
    './src/data/tile_wall_bottom.cpp',
    './src/data/tile_wall_left.cpp',
    './src/data/tile_wall_top_left.cpp',
    './src/data/tile_wall_right_left.cpp',
    './src/data/tile_wall_top_right_left.cpp',
    './src/data/tile_cell_selected.cpp',
    './src/data/tile_at.cpp',
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
