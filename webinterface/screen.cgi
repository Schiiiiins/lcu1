#!/bin/tclsh
source cgi.tcl
source common.tcl

proc cmd_download {} {
    exec fbshot /tmp/screen.png
    set fd [open "/tmp/screen.png"]
    catch {fconfigure $fd -translation binary}
    catch {fconfigure $fd -encoding binary}
    puts "Content-Type:image/png\n"
    #puts "Content-Disposition:attachment;filename=screen.png\n"
    catch {fconfigure stdout -translation binary}
    catch {fconfigure stdout -encoding binary}
    while { ! [eof $fd]} {
        puts -nonewline [read $fd 65536]
    }
    close $fd
    
}

cgi_eval {
cgi_debug -off
cgi_input

set cmd download
catch { import cmd }
cmd_$cmd
}

