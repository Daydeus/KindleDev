project(
    'kindleDev',
    'cpp',
    version: 'v1.0.0',
    default_options: ['cpp_std=c++17'],
    meson_version: '>=1.1'
)

# Define dependencies we want
gtk_dep = dependency('gtk+-2.0')

# Check for conditional compilation
if get_option('kindle_build')
    add_project_arguments('-DKINDLE_BUILD', language: 'cpp')
endif


####################
# Project definition
####################
sources = files(
    './src/main.cpp',
    './src/viewPort.cpp',
    './src/dungeonCell.cpp',
    './src/tile_null.cpp',
    './src/tile_floor_base.cpp',
    './src/tile_wall_top.cpp',
    './src/tile_wall_right.cpp',
    './src/tile_wall_top_right.cpp',
    './src/tile_wall_bottom.cpp',
    './src/tile_wall_left.cpp',
    './src/tile_wall_top_left.cpp',
    './src/tile_wall_right_left.cpp',
    './src/tile_wall_top_right_left.cpp',
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
