require 'mkmf'
require 'rbconfig'

unless RUBY_PLATFORM =~ /linux/
  abort 'xrb only supports GNU/Linux at the moment.'
end

LIB_DIR     = RbConfig::CONFIG['libdir']
INCLUDE_DIR = RbConfig::CONFIG['includedir']

HEADER_DIRS = [
  '/opt/local/include',
  '/usr/local/include',
  '/usr/include',
  INCLUDE_DIR,
  __dir__,
]

LIB_DIRS = [
  '/opt/local/include',
  '/usr/local/include',
  '/usr/include',
  LIB_DIR,
]

def xrb_require_library(lib)
  if have_library(lib)
    dir_config("lib#{lib}", HEADER_DIRS, LIB_DIRS)
  else
    abort "Couldn't find '#{lib}'. xrb needs '#{lib}'. Is '#{lib}' installed?"
  end
end

xrb_require_library 'xcb'
xrb_require_library 'xcb-util'
