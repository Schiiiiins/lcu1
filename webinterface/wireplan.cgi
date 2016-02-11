#!/bin/tclsh
source cgi.tcl
source common.tcl

load tclrpc.so

set URL xml://127.0.0.1:2005


proc cmd_show {} {
    global env URL

    cgi_head {
        http_equiv Content-Type "text/html; charset=utf-8"
        title "LCU1 - Verdrahtungsplan"
    }
    
    body {
        h1 "Verdrahtungsplan"
        set connections [xmlrpc $URL listValueConnections]
        
        foreach connection_info $connections {
            array set conn $connection_info
            set connections_by_channel($conn(Device):$conn(ChannelIndex)) $connection_info
            lappend device_channels($conn(Device)) $conn(ChannelIndex)
        }
        
        foreach device [lsort [array names device_channels]] {
            hr
            set channel_list $device_channels($device)
            array_clear conn
            array set conn $connections_by_channel($device:[lindex $channel_list 0])
            set display_name $conn(DeviceDisplayName)
            h2 "Logisches Gerät $display_name"
            if [info exist conn(XmlRpcDevice)] {
                puts "Zugeordnet zum physikalischen Gerät: $conn(XmlRpcDevice)"
            }
            table cellpadding=5 {
                table_row {bgcolor="#C0C0C0"} {
                    th "XML-RPC Kanal"
                    th "Anschluss"
                    th "Eingang/Ausgang"
                    th "Logiksignal"
                    th "Beschreibung"
                }
                foreach channel_index [lsort -integer $channel_list] {
                    array set conn $connections_by_channel($device:$channel_index)
                    table_row {
                        td "$channel_index"
                        td "$conn(Terminal)"
                        if { "$conn(Direction)" == "Input" } {
                            td "Eingang"
                        } else {
                            td "Ausgang"
                        }
                        td "$conn(Alias)"
                        td "$conn(DisplayName)"
                    }                    
                }
            }
        }
    }
}


cgi_eval {
if [file exists /etc/config/cgi_debug] {cgi_debug -on}
cgi_input

set cmd show
catch { import cmd }
cmd_$cmd
}

 
