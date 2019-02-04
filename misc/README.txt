installscript.qs
----------------

An example script which I have sent to customers before which effectively does a silent installation of Qt. What they need to do is run this like:

  --script installscript.qs --verbose getcomponents=true

and that will give a list of components that they can get from the list and put the ones they want to install into the ComponentSelectionPageCallback function. There is an example there that can be used as a guide, then they can install like:

  --script installscript.qs targetdir=/path/to/qt

with the targetdir option indicating where they want Qt to be installed.
