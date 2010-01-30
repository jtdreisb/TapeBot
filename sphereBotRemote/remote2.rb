#!/usr/bin/env ruby
#
# This file is gererated by ruby-glade-create-template 1.1.4.
#
require 'libglade2'

class SpherebotremoteGlade
  include GetText

  attr :glade
  
  def initialize(path_or_data, root = nil, domain = nil, localedir = nil, flag = GladeXML::FILE)
    bindtextdomain(domain, localedir, nil, "UTF-8")
    @glade = GladeXML.new(path_or_data, root, domain, localedir, flag) {|handler| method(handler)}
    
  end
  
  def on_window_key_release_event(widget, arg0)
    puts "on_window_key_release_event() is not implemented yet."
  end
  def on_rightButton_released(widget)
    puts "on_rightButton_released() is not implemented yet."
  end
  def on_stopButton_pressed(widget)
    puts "on_stopButton_pressed() is not implemented yet."
  end
  def on_connectButton_clicked(widget)
    puts "on_connectButton_clicked() is not implemented yet."
  end
  def on_leftButton_released(widget)
    puts "on_leftButton_released() is not implemented yet."
  end
  def on_reverseButton_pressed(widget)
    puts "on_reverseButton_pressed() is not implemented yet."
  end
  def on_forwardButton_pressed(widget)
    puts "on_forwardButton_pressed() is not implemented yet."
  end
  def on_leftButton_pressed(widget)
    puts "on_leftButton_pressed() is not implemented yet."
  end
  def on_rightButton_pressed(widget)
    puts "on_rightButton_pressed() is not implemented yet."
  end
  def on_window_key_press_event(widget, arg0)
    puts "on_window_key_press_event() is not implemented yet."
  end
  def on_window_destroy_event(widget, arg0)
    puts "on_window_destroy_event() is not implemented yet."
  end
  def on_reverseButton_released(widget)
    puts "on_reverseButton_released() is not implemented yet."
  end
  def on_motorSpeed_drag_end(widget, arg0)
    puts "on_motorSpeed_drag_end() is not implemented yet."
  end
  def on_forwardButton_released(widget)
    puts "on_forwardButton_released() is not implemented yet."
  end
end

# Main program
if __FILE__ == $0
  # Set values as your own application. 
  PROG_PATH = "spherebotremote.glade"
  PROG_NAME = "YOUR_APPLICATION_NAME"
  SpherebotremoteGlade.new(PROG_PATH, nil, PROG_NAME)
  Gtk.main
end
