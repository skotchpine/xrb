load File.expand_path('../include/extconf.rb', __dir__)

xrb_require_library 'xcb-keysyms'
xrb_require_library 'X11'

$srcs = [File.expand_path('./keyboard.c', __dir__)]

create_makefile('keyboard')
