#!/usr/bin/env ruby
#Awesome ruby
# http://xkcd.com/413/
#puts e.to_a #To array


#When asked to eliminate an if make a default value to be returned from a hash table

require 'rubygems'
require 'serialport'

class Xiphos
   def initialize( p = '/dev/rfcomm0',  b = 9600)
       @ser = SerialPort.new( p,b,8 )
       @motorSpeed = 127
   end
   
   def ledOn
      @ser.putc 1
   end
   
   def ledOff
      @ser.putc 2
   end
   
   def lcdOn
      @ser.putc 5
   end
   
   def lcdOff
      @ser.putc 6
   end
   
   def clearScreen
      @ser.putc 7
   end
   
   def nextLine
      @ser.putc 8
   end
   
   def button?
      @ser.putc 9
      @ser.getc
   end
   
   def softReset
      @ser.putc 11
   end
   
   def brake
      @ser.putc 12
      @motorSpeed = 127      
      puts "Braking…"
   end
   def brake1
      @ser.putc 13
      @motorSpeed = 127      
      puts "Braking…"
   end
   
   def move( speed )
      if( speed == nil)
         speed = 127
      end
      puts "Now Moving at #{speed}"
      @ser.putc 31
      @ser.putc speed.to_i
      @ser.putc speed.to_i 
      
   end
   def fullForward
      @motorSpeed = 255
      @ser.putc 31
      @ser.putc @motorSpeed
      @ser.putc @motorSpeed
   end
   def fullForward1
      @motorSpeed = 255
      @ser.putc 33
      @ser.putc @motorSpeed
      @ser.putc @motorSpeed
   end
   def fullReverse
      @motorSpeed = 0
      @ser.putc 31
      @ser.putc @motorSpeed
      @ser.putc @motorSpeed
   end
   def fullReverse
      @motorSpeed = 0
      @ser.putc 33
      @ser.putc @motorSpeed
      @ser.putc @motorSpeed
   end
   def servo0( pos )
      @ser.putc 30
      @ser.putc 0
      @ser.putc pos.to_i
      @ser.putc 30
      @ser.putc 1
      @ser.putc(255-pos.to_i)
      puts "#{(255-pos.to_i)}"
   end
   def faster
      @motorSpeed += 15      
      @ser.putc 31
      @ser.putc @motorSpeed
      @ser.putc @motorSpeed   
   end
   def slower
      @motorSpeed -= 15      
      @ser.putc 31
      @ser.putc @motorSpeed
      @ser.putc @motorSpeed   
   end
   def print( s )
      @ser.putc 22
      s.each_char { |c|
         @ser.putc c
      }
      @ser.putc 0
   end
   
end
if __FILE__ == $0
   x = Xiphos.new
   puts("Xiphos has been connected")
   x.ledOn
   while true
      print('> ')
      command = gets.split
      case( command[0] )
         when 'ledOn'         : x.ledOn
         when 'ledOff'        : x.ledOff
         when 'lcdOn'         : x.lcdOn
         when 'lcdOff'        : x.lcdOff
         when 'clearScreen'   : x.clearScreen
         when 'nextLine'      : x.nextLine
         when 'button?'       : puts("Button = #{x.button?}")
         when 'r'             : x.softReset
         when 'move', 'm'     : x.move(command[1])
         when 'f'             : x.faster
         when 'slo'           : x.slower
         when 'p'             : x.fullForward
         when 'g'             : x.fullForward1
         when 'h'             : x.fullReverse1
         when 'o'             : x.fullReverse
         when 's'             : x.servo0(command[1])
         when 'brake', 'b'    : x.brake
         when 'b1'            : x.brake1
         when 'print'         :  
            if command.length > 1
               x.print(command[1])
            end
         else puts(" Unrecognized Command ")
      end
   endend
