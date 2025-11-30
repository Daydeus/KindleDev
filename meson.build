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
    './src/global.cpp',
    './src/actor.cpp',
    './src/dungeonCell.cpp',
    './src/dungeonGeneration.cpp',
    './src/menuBox.cpp',
    './src/tile.cpp',
    './src/touchGesture.cpp',
    './src/viewPort.cpp',
    './src/main.cpp',

### C-Source files for tile graphics
    './src/data/tilesetColorFill.cpp',
    './src/data/tilesetBorder.cpp',
    './src/data/tilesetDungeonCave.cpp',
    './src/data/tilesetMenuBoxSettings.cpp',
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
