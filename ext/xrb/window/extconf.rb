load File.expand_path('../include/extconf.rb', __dir__)

$srcs = [File.join(__dir__, 'window.c')]

create_makefile('window')
