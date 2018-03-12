load File.join(__dir__, 'lib/xrb.rb')

class << Xrb
  def launch
    system 'dmenu_run &'
  end

  def bootstrap
    f = Xrb::Window.all.find { |w| w.wm_class == 'st' }
    f = f.focus
    f.monitor.window_to_center(f)
  end
end

class Xrb::Window
  def self.user
    all.reject do |w|
      %w[compton lemonbar].include?(w.wm_class)
    end
  end

  def anchor
    [@x, @y]
  end

  def center
    [(@width / 2) + @x, (@height / 2) + @y]
  end

  def move_rel(x, y)
    move(@x + x, @y + y)
  end

  def resize_rel(width, height)
    resize(@width + width, @height + height)
  end

  def teleport_rel(x, y, width, height)
    teleport(@x + x, @y + y, @width + width, @height + height)
  end

  def monitor
    Xrb::Monitor.all.find do |m|
      center.first > m.x and center.first < (m.x + m.width) and
        center.last > m.y and center.last < (m.y + m.height)
    end
  end
end

class Xrb::Monitor
  def anchor
    [@x, @y]
  end

  def center
    [(@width / 2) + @x, (@height / 2) + @y]
  end

  def window_to_center(w)
    w.teleport \
      @x + ((@width - w.width) / 2),
      @y + ((@height - w.height) / 2),
      w.width,
      w.height
  end

  def window_to_full(w, gap = 0)
    w.teleport x + gap, y + gap, width - (gap * 2), height - (gap * 2)
  end
end

Xrb.bootstrap
