load File.expand_path('../include/extconf.rb', __dir__)

$srcs = [File.join(__dir__, 'core.c')]

create_makefile('core')
