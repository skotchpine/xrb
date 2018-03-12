load File.expand_path('../include/extconf.rb', __dir__)

$srcs = [File.expand_path('./event.c', __dir__)]

create_makefile('event')
