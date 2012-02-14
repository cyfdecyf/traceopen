#!/usr/bin/env ruby

require 'pathname'

STDIN.each_line do |file|
  puts Pathname.new(file).cleanpath
end
