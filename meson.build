project(
  'desktoggle',
  'c',
  version: '1.7.1',
  license: 'GPL3+',
)

a2x = find_program('a2x')
gzip = find_program('gzip')

executable(
  'desktoggle',
  'desktoggle.c',
  dependencies: dependency('x11'),
  install: true,
)

manpage = custom_target(
  'desktoggle(1)',
  input: 'desktoggle.1.asciidoc',
  output: 'desktoggle.1',
  command: [
    a2x,
    '--doctype=manpage', '--format=manpage',
    ''.join(['--destination-dir=@OUTDIR@']), '@INPUT@',],
  install: false,
)

custom_target(
  'compressed desktoggle(1)',
  input: manpage,
  output: 'desktoggle.1.gz',
  command: [gzip, '-9', '--force', '--keep', '--no-name', '@INPUT@',],
  install: true,
  install_dir: join_paths(get_option('mandir'), 'man1',),
)

# vim: set et sts=2 sw=2 ts:2:
