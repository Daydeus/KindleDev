project(
    'kindleDev',
    'cpp',
    version: 'v1.0.0',
    default_options: ['cpp_std=c++17'],
    meson_version: '>=1.1'
)

# Define dependencies we want
gtk_dep = dependency('gtk+-2.0')

####################
# Project definition
####################
sources = files(
    './src/main.cpp',
    './src/example.cpp'
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
