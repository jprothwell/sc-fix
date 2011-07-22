#!/usr/bin/ruby

# This script will generate the flash programmer ramrun files for a list
# of FF targets and a list of Eng board plus flash model targets.

ff_targets = [
    "CT_TARGET=gallite_g33",
]

#    "CT_TARGET=gallite_g33 FLSH_MODEL=flsh_m36l0r7050u3zs",

#eng_targets = [
#    "CT_TARGET=greenstone_eng_ct1010 CT_ROMULATOR=no FLSH_MODEL=flsh_k5l2731caa_d770",
#]

Dir.chdir(ENV["SOFT_WORKDIR"]+"/toolpool/flash_programmer")

puts "Generating all the flash programmer files..."

# Flash programmers for Form Factor boards.
ff_targets.each{ |tgt|
    # Flash programmer for Host.
    puts "FP over Host for #{tgt}..."
    `make FASTPF_USE_USB=0 #{tgt} CT_USER=ADMIN CT_RELEASE=release clean lod flshprog_install_ff flshprog_clean`

    # Flash programmer for USB.
    #puts "FP over USB  for #{tgt}..."
    #`make FASTPF_USE_USB=1 #{tgt} CT_USER=ADMIN CT_RELEASE=release clean lod flshprog_install_ff flshprog_clean`
}

# Flash programmers for Engineering boards.
#eng_targets.each{ |tgt|
#    # Flash programmer for Host.
#    puts "FP over Host for #{tgt}..."
#    `make FASTPF_USE_USB=0 #{tgt} CT_USER=ADMIN CT_RELEASE=release clean lod flshprog_install_eng flshprog_clean`
#
#    # Flash programmer for USB.
#    puts "FP over USB  for #{tgt}..."
#    `make FASTPF_USE_USB=1 #{tgt} CT_USER=ADMIN CT_RELEASE=release clean lod flshprog_install_eng flshprog_clean`
#}

puts "Done."
