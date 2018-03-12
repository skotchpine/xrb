load File.expand_path('../include/extconf.rb', __dir__)

xrb_require_library 'xcb-randr'

$srcs = [File.expand_path('./monitor.c', __dir__)]

create_makefile('monitor')
