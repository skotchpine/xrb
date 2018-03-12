%w[
  xrb/core
  xrb/window
  xrb/monitor
  xrb/pointer
  xrb/keyboard
  xrb/event
]
  .each(&method(:require))

Xrb::Base.connection = Xrb::Connection.new
