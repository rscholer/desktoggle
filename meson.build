project(
  'desktoggle',
  'c',
  version: '1.7.1',
  license: 'GPL3+',
)

conf_project = configuration_data()
conf_project.set('PROJECT_AUTHOR', 'Raphael Scholer')
conf_project.set('PROJECT_AUTHOR_EMAIL', 'rascholer@gmail.com')

deps_desktoggle = dependency('x11')

prog_gzip = find_program('gzip')
prog_rst2man = find_program('rst2man', 'rst2man.py')

executable(
  'desktoggle',
  'desktoggle.c',
  dependencies: deps_desktoggle,
  install: true,
)

manpage_template = configure_file(
  input: 'desktoggle.1.rst.in',
  output: '@BASENAME@',
  configuration: conf_project,
)

manpage_raw = custom_target(
  'desktoggle(1)',
  input: manpage_template,
  output: '@BASENAME@',
  command: [
    prog_rst2man,
    '@INPUT@',
    '@OUTPUT@',
  ],
  install: false,
)

custom_target(
  'compressed desktoggle(1)',
  input: manpage_raw,
  output: '@PLAINNAME@.gz',
  capture: true,
  command: [
    prog_gzip,
    '-9', '--force', '--keep', '--no-name', '--stdout',
    '@INPUT@',
  ],
  install: true,
  install_dir: join_paths(get_option('mandir'), 'man1',),
)

# vim: set et sts=2 sw=2 ts:2:
