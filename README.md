# picnic

A picture viewer allows you to flip (or better roll) thru your images by using OpenGL, thumbnail services and Dbus.
Allows cropping a image, and saving in formats supported by Gdk/Gtk.

![Picnic](picnic.png "picnic")

![PicnicWin](picview.png "picnic view window")

Build with autotools, requires genericGlm and genericImg (see there for some basic build infos).

By no means that is intended to be complete, or help you to go thru anything more than a hundred pictures.

Enjoy it, you may improve it, or leave it.

<pre>
configure:<br>
     allow --with-gles using GL ES 3 e.g. useful on Raspi's
</pre>

Developer info:
<pre>
# if you need to regenerate the interface the following might be helpful:
#       thumbnail1.xml can be create with dfeet by calling introspection.
#	the generated code needs manual tuning e.g. add private message member and closing brace, remove path for imports
#	gdbus-codegen-glibmm2 --generate-cpp-code=${HOME}/src/Picnic/cpp/thumbnail1 ${HOME}/csrc/Picnic/res/thumbnail1.xml
#???	gdbus-codegen-glibmm2 --generate-cpp-code=${HOME}/csrc/Picnic/cpp/cache1 ${HOME}/csrc/Picnic/res/cache1.xml
</pre>