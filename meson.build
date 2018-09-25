project(
  'desktoggle',
  'c',
  version: '1.8',
  license: 'GPL3+',
)

conf_desktoggle = configuration_data()
conf_desktoggle.set_quoted('PROJECT_VERSION', meson.project_version())

deps = {
  'x11': dependency('x11'),
}

progs = {
  'gzip': find_program('gzip'),
  'rst2man': find_program('rst2man', 'rst2man.py'),
}

configure_file(
  input: 'desktoggle.h.in',
  output: '@BASENAME@',
  configuration: conf_desktoggle,
)

executable(
  'desktoggle',
  'desktoggle.c',
  dependencies: deps['x11'],
  install: true,
)

manpage_raw = custom_target(
  'desktoggle(1)',
  input: 'desktoggle.1.rst',
  output: '@BASENAME@',
  command: [
    progs['rst2man'],
    '@INPUT@',
    '@OUTPUT@'
  ],
  install: false,
)

custom_target(
  'compressed desktoggle(1)',
  input: manpage_raw,
  output: '@PLAINNAME@.gz',
  capture: true,
  command: [
    progs['gzip'],
    '-9', '--force', '--keep', '--no-name', '--stdout',
    '@INPUT@'
  ],
  install: true,
  install_dir: join_paths(get_option('mandir'), 'man1'),
)

# vim: set et sts=2 sw=2 ts=2:
