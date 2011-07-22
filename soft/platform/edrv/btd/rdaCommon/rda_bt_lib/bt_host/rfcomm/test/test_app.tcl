proc frames_init {} {
    wm title . "Parthus Technologies Linux Host Tester"
    frame .f1
}

proc create_buttons {} {
    frame .f4 -borderwidth 1 -relief raised

    button .f4.b4 -text "Exit" -command exit
    pack append .f4 .f4.b4 {left}
    pack .f4
}

proc do_inquiry {} {
    toplevel .inq
    wm title .inq "Inquiry"
    frame .inq.f1
    listbox .inq.f1.l1
    button .inq.f1.b1 -text "Start Inquiry" -command ccode_inquiry 
    button .inq.f1.b2 -text "Set BD Address" -command get_addr_from_box
    button .inq.f1.b3 -text "OK" -command "destroy .inq"
    pack .inq.f1.l1 -in .inq.f1 -side top
    pack .inq.f1.b1 -in .inq.f1 -side left
    pack .inq.f1.b2 -in .inq.f1 -side left
    pack .inq.f1.b3 -in .inq.f1 -side left

    pack .inq.f1
    grab .inq    
}

proc get_addr_from_box {} {
    
    set num [.inq.f1.l1 curselection]
    set addr [.inq.f1.l1 get $num]
    process_bdaddr $addr

}

proc get_bdaddr {} {
    toplevel .gbd
    wm title .gbd "Set Remote BDAddress"
    frame .gbd.f1
    entry .gbd.f1.e1
    label .gbd.f1.l1 -text "Please enter Bluetooth Device Address"
    label .gbd.f1.l2 -text "Ox"
    button .gbd.f1.b1 -text "OK" -command "process_box"
    button .gbd.f1.b2 -text "Cancel" -command "destroy .gbd"
    pack .gbd.f1.l1
    pack append .gbd.f1 .gbd.f1.l2 {left} .gbd.f1.e1 {left}
    pack .gbd.f1.b2 -side bottom
    pack .gbd.f1.b1 -side bottom
    pack .gbd.f1
    grab .gbd
}

proc process_box {} {
    set bd_addr [.gbd.f1.e1 get]
    process_bdaddr $bd_addr
}

proc process_bdaddr { bdaddr } {
    global rem_bdaddr

    set rem_bdaddr $bdaddr
    .f7.f2.l2 configure -text 0x$rem_bdaddr
    ccode_bdaddr
}

proc init_frames {} {
    global .top1 .top2
        
    init_frame .top1 
    wm title .top1 "RFCOMM IUT"
    init_frame .top2 
    wm title .top2 "RFCOMM Tester"
}
proc init_frame { top } {
    
    toplevel $top

    frame $top.output
    frame $top.output.buttons

    button $top.output.buttons.clear -text "Clear" -command "output_clear_window $top"
    button $top.output.buttons.save -text "Save..." -command "output_save $top"
    button $top.output.buttons.kill -text "Finish" -command "output_kill_task $top"

    pack $top.output.buttons.clear -side left -expand 1
    pack $top.output.buttons.save -side left -expand 1
    pack $top.output.buttons.kill -side left -expand 1
    pack $top.output.buttons -side top -fill x
    text $top.output.text -wrap word -state disabled -yscrollcommand [list $top.output.scroll set]

    scrollbar $top.output.scroll -orient vertical -command [list $top.output.text yview]
    pack $top.output.text -side left -fill both -expand 1 -in $top.output
    pack $top.output.scroll -side right -fill y -in $top.output
    pack $top.output

}

proc start_iut { test_num } {
    run_command nbpage_build_handle_output1 iut/rfcomm_iut $test_num
}

proc start_tester { test_num } {
    run_command nbpage_build_handle_output2 tester/rfcomm_tester $test_num
}

proc nbpage_build_handle_output1 { mesg } {
    global .top1

    if { $mesg == "" } {
	output_add_text .top1 "Build has finished.\n"
#	nbpage_build_control_buttons 1
    } else {
	output_add_text .top1 "$mesg\n"
    }
}

proc nbpage_build_handle_output2 { mesg } {
    global .top2

    if { $mesg == "" } {
	output_add_text .top2 "Build has finished.\n"
#	nbpage_build_control_buttons 1
    } else {
	output_add_text .top2 "$mesg\n"
    }
}
proc output_clear_window { top } {
    global $top

    $top.output.text configure -state normal
    $top.output.text delete 1.0 end
    $top.output.text configure -state disabled

}

proc output_add_text { top msg } {
    global $top 
    global .top1 
    global .top2
    
    $top.output.text configure -state normal
    $top.output.text insert end $msg
    $top.output.text configure -state disabled
    $top.output.text see end
}

proc output_kill_task { top } {
    catch { exec -- /usr/bin/killall -q rfcomm_iut } 
    catch { exec -- /usr/bin/killall -q rfcomm_tester } 
}



proc run_command { callback command args } {

    # Start up the specified command. Actually the main command is run such that
    # both stdout and stderr are piped into cat, thus making sure that the
    # stderr output is not discarded.
    set result [catch {	set pipe [open  "|[concat $command $args] 2>@ stdout" "r"] } message]
    if { $result != 0 } {
	warning "Failed to execute $command: $message"
	return
    }

    # Now set the pipe to non-blocking, to prevent the whole GUI from
    # locking up by accident.
    fconfigure $pipe -blocking 0

    # Set up an event handler to read back any output.
    fileevent $pipe readable [list handle_command_output $pipe $callback]
}

#
# This routine gets invoked whenever there may be data waiting on the
# pipe.
#
proc handle_command_output { pipe callback } {

    set data  ""
    set count [gets $pipe data]
    if { $count > 0 } {
	$callback $data
    } elseif { [eof $pipe] } {
	catch { close $pipe }
	$callback ""
    }
}

proc output_save { top } {

    
    set filename [tk_getSaveFile -initialdir . -initialfile pkgconf.log \
	    -parent $top ]
    
    if { $filename == "" } {
	return
    }
    
    if { [catch {
	set file [open $filename w]
	puts $file [$top.output.text get 1.0 "end - 1 chars"]
	close $file
    } message] != 0 } {
	warning "Failed to save build output to $filename: $message"
    }
}

