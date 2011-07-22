#!/usr/bin/ruby

# This script will generate the t-flash programmer ramrun files for a list
# of FF targets.

ff_target = [
    "gallite_g33","gallite_j23",
]

puts "Generating all the t-flash programmer files..."

ff_target.each{ |target|
    # T-flash programmer.
    puts "TFP for #{target}..."
    `make CT_TARGET=#{target} CT_USER=ADMIN CT_RELEASE=release clean lod flshprog_install`
    
}

puts "Done."
