project('picnic', 'c', 'cpp'
    , default_options: ['c_std=gnu11'
                       , 'cpp_std=c++20'
                       , 'buildtype=debugoptimized'
                       , 'warning_level=3']
    , license : 'GPL-3.0'
    , meson_version: '>=1.3.0'
    , version: '0.1')

fs = import('fs')

pkgdatadir = join_paths(get_option('prefix'), get_option('datadir'), meson.project_name())
pkglocaledir = join_paths(get_option('prefix'), get_option('datadir'), 'locale')
pkgsrcdir = fs.relative_to( meson.global_source_root(), meson.current_build_dir()) + '/src'
add_project_arguments('-DPACKAGE_DATA_DIR="' + pkgdatadir + '"', language: ['cpp','c'])
add_project_arguments('-DPACKAGE="' + meson.project_name() + '"', language: ['cpp','c'])
# this is to recreate the conventions from autotools
add_project_arguments('-DPACKAGE_SRC_DIR="' + pkgsrcdir + '"', language: ['cpp','c'])
#add_project_arguments('-DGETTEXT_PACKAGE="' + meson.project_name() + '"', language: ['c'])
#add_project_arguments('-DPACKAGE_LOCALE_DIR="' + pkglocaledir + '"', language: ['cpp','c'])


disable_flags = [
     '-Wno-unused-parameter'    # limit the compiler noisiness
    ,'-Wno-overlength-strings'  # this matters only if you are pedantic
]
cc = meson.get_compiler('c')
foreach cflag : disable_flags
    if cc.has_argument(cflag)
        add_project_arguments(cflag, language: ['c', 'cpp'])
    endif
endforeach

conf = configuration_data()
conf.set_quoted('PICNIC_VERSION', meson.project_version())     # Surround the version in quotes to make it a C string
conf_file = configure_file(output : meson.project_name() + '_config.h'
               , configuration : conf)

include_dirs = include_directories('gen'
                                , fs.parent(conf_file))

thread_deps     = dependency('threads')
dbus_deps       = dependency('dbus-1 dbus-glib-1')
genericimg_deps = dependency('genericimg', version :'>= 0.4.0')
genericglm_deps = dependency('genericglm', version :'>= 0.3.1')
openssl_deps    = dependency('openssl')
deps = [
      genericimg_deps
    , genericglm_deps
    , dbus_deps
    , openssl_deps
    , thread_deps
    ]

subdir('gen')
subdir('src')
subdir('test')
subdir('res')
sources += app_resources

project_targt = executable(meson.project_name()
    , sources
    , dependencies         : deps
    , include_directories  : include_dirs
    , link_with            : libthumbnail1
    , install              : true
    , win_subsystem        : 'windows'  # disable this to see console output for windows
    )
