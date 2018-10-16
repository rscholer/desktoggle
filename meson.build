project(
  'desktoggle',
  'c',
  version: '1.8',
  license: 'GPL3+',
)

add_project_arguments(
  '-DPROJECT_VERSION="@0@"'.format(meson.project_version()),
  language: 'c',
)

deps = {
  'x11': dependency('x11'),
}

executable(
  'desktoggle',
  'desktoggle.c',
  dependencies: deps['x11'],
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
