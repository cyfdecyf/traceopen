#!/usr/bin/env ruby

require 'pathname'

STDIN.each_line do |file|
  pa = Pathname.new(file.chomp).cleanpath
  puts pa if pa.exist?
end
