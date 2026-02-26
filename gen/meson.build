

# the custom_targets are listed as reference
#   if you want to know how it's done,
#   but may need some tweaking (e.g. remove comment for build_by_def...)
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
# these will be generate into build, the above will placed into build/gen as expected
gen_cpp = custom_target('gen-cpp'
#        , build_by_default: true
        , output        : intf_dest
        , input         : PICNIC_INTF_XML_BASE
        , command       : [gdbus_codegen
                          , '--generate-cpp-code=' + PICNIC_INTF_PREFIX
                          , '@INPUT@']
        , depends       : gen_xml)

intf_files = files(intf_dest)
libthumbnail1 = static_library('thumbnail1'
        , intf_files
        , dependencies : deps
        , include_directories : include_dirs)


