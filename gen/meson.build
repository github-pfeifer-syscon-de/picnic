
# the custom_targets are listed as reference
#   if you want to know how it's done
#   maybe gnome.gdbus_codegen() see https://mesonbuild.com/Gnome-module.html#gnome-module
#     is simpler 
PICNIC_INTF_PREFIX='thumbnail1'
PICNIC_INTF_XML_BASE=PICNIC_INTF_PREFIX + '.xml'
DBUS_INTF_NAME='org.freedesktop.thumbnails.Thumbnailer1'
DBUS_INTF_PATH='/org/freedesktop/thumbnails/Thumbnailer1'

# gdbus may come with the glib package (or maybe some developer part?)
gdbus = find_program('gdbus', required: false)
# howto get gdbus-codegen-glibmm3 see README.md
gdbus_codegen = find_program('gdbus-codegen-glibmm3', required: false)

gen_xml = custom_target('gen-xml'
        , output        : [PICNIC_INTF_XML_BASE]
        , capture       : true                     # write stdout to output
        , command       : [gdbus, 'introspect'
                          ,'-e','-d', DBUS_INTF_NAME
                          ,'-o', DBUS_INTF_PATH
                          ,'-x'])
intf_dest = [
	 PICNIC_INTF_PREFIX + '_common.cpp'
	,PICNIC_INTF_PREFIX + '_proxy.cpp'
	,PICNIC_INTF_PREFIX + '_stub.cpp'
        ]

# ensure the output is generated into gen so the lib will find it ...
rel_build = fs.relative_to(meson.current_build_dir(), meson.global_build_root())
cpp_gen_target = rel_build + '/' + PICNIC_INTF_PREFIX
gen_cpp = custom_target('gen-cpp'
        , output        : [
             PICNIC_INTF_PREFIX + '_common.cpp'
            ,PICNIC_INTF_PREFIX + '_proxy.cpp'
            ,PICNIC_INTF_PREFIX + '_stub.cpp'
            ]
        , input         : gen_xml
        , command       : [gdbus_codegen
                          , '--generate-cpp-code=' + cpp_gen_target
                          , '@INPUT@'])

intf_files = files(intf_dest)
libthumbnail1 = static_library('thumbnail1'
        , intf_files         # switch comments to use distributed interface
#        , gen_cpp             # switch comments to rebuild interface will be placed in build/gen
        , dependencies : deps
        , include_directories : include_dirs)


# for reference
#   custom_targets command will always run from build dir.
#   for the xml-part this is fine as the output will be prefixed with gen by meson.
#   for the cpp-part the target must be constructed so the files ends up in gen.
