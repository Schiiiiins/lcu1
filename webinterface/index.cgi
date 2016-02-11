#!/bin/tclsh
source cgi.tcl
source common.tcl

proc read_var { filename varname} {
    set fd [open $filename r]
    set var ""
    if { $fd >=0 } {
        while { [gets $fd buf] >=0 } {
            if [regexp "^ *$varname *= *(.*)$" $buf dummy var] break
        }
        close $fd
    }
    return $var
}
    

proc put_page {} {
    http_head
    html {
        head {
            title "ELV LCU1"
        }
        body {
            set version [read_var /etc/VERSION VERSION]
            set build_time [read_var /etc/VERSION BUILD_TIME]
            h1 "ELV LCU 1"
            puts "Firmware version: $version"
            br
            puts "Build date: [clock format $build_time -format {%d.%m.%Y %H:%M:%S %Z}]"
            br
           	h3 [url "Firmware update" fwupdate.cgi]
            br
           	h3 [url "XML-RPC-Geräte konfigurieren" devconfig.cgi]
            br
           	h3 [url "Logische und physikalische Geräte verknüpfen" busdevices.cgi]
            br
           	h3 [url "Verdrahtungsplan anzeigen" wireplan.cgi]
        }
    }
}

cgi_eval {
#cgi_debug -on
cgi_input

put_page

}

