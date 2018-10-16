project(
  'desktoggle',
  'c',
  version: '1.8',
  license: 'GPL3+',
  meson_version: '>=0.47.0',
  default_options: ['c_std=c99'],
)

add_project_arguments(
  '-DPROJECT_VERSION="@0@"'.format(meson.project_version()),
  language: 'c',
)

executable(
  'desktoggle',
  'desktoggle.c',
  dependencies: dependency('x11'),
  install: true,
)

custom_target(
  'desktoggle.1',
  input: 'desktoggle.1.rst',
  output: 'desktoggle.1',
  command: [
    find_program('rst2man', 'rst2man.py'),
    '@INPUT@', '@OUTPUT@'
  ],
  install: true,
  install_dir: join_paths(get_option('mandir'), 'man1'),
)

# vim: set et sts=2 sw=2 ts=2:
