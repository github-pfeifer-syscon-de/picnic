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
   to regenerate the interface the following might be helpful:
     this is somewhat incomplete as this wasn't simple on the first try, and i still have some difficulties to reproduce this
       thumbnail1.xml can be create with dfeet by calling introspection.
	the generated code needs manual tuning e.g. add private message member and closing brace, remove path for imports
	clone https://github.com/Pelagicore/gdbus-codegen-glibmm
        ensure the python dependencies are available e.g. python-setuptools, python-j2cli
        use "python setup.py install --record egg-files.txt"
        this allows unsinstall with "cat egg-files.txt | rm -rf"
	gdbus-codegen-glibmm3 --generate-cpp-code=thumbnail1 thumbnail1.xml
</pre>