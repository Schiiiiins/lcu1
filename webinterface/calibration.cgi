#!/bin/tclsh
source ../cgi.tcl
source ../common.tcl

load tclrpc.so

set URL xml://127.0.0.1:2005


proc show_options { connectionInfoVar } {
    global env
    
    upvar $connectionInfoVar connectionInfo
    
    set interfaceUrl $connectionInfo(XmlRpcInterfaceUrl)
    set interfaceId $connectionInfo(XmlRpcInterfaceId)
    set channelAddress $connectionInfo(XmlRpcChannel)
    
    array set paramsetDescription [xmlrpc $interfaceUrl getParamsetDescription $channelAddress MASTER]
    
    set hasCalibrationParameters 0
    foreach parameterName [array names paramsetDescription] {
        array set parameterDescription $paramsetDescription($parameterName)
        if { ($parameterDescription(FLAGS) & 0x04) == 0 } { set hasCalibrationParameters 1 }
    }
    if { !$hasCalibrationParameters } return
    
    array set paramsetValues [xmlrpc $interfaceUrl getParamset $channelAddress MASTER]
            
    set bgcolors "#C0FFC0 #A0FFA0"
    set i 0
    foreach parameterName [array names paramsetDescription] {
        array set parameterDescription $paramsetDescription($parameterName)
        
        if { ($parameterDescription(FLAGS) & 0x04) } continue
        
        set type $parameterDescription(TYPE)
        set unit $parameterDescription(UNIT)
        set display_name $connectionInfo(Alias)
        set option $connectionInfo(Alias)_$parameterName
        set help_text ""
        set value $paramsetValues($parameterName)
        
        table_row bgcolor=[lindex $bgcolors [expr $i%2]] {
            td $display_name
            td $parameterName
            switch $type {
                ENUM {
                    set enum_list [lindex $type_spec 1 ]
                    table_data {
                        cgi_select ${option}_value id=\"${option}_value\" {
                            set j 0
                            foreach enum_option $enum_list {
                                if { $j == $value } {
                                    set selected {selected="true"}
                                } else {
                                    set selected ""
                                }
                                cgi_option $enum_option value=$j $selected
                                incr j
                            }
                        }
                    }
                }
                BOOLEAN {
                    set enum_list {Off On}
                    table_data {
                        cgi_select ${option}_value id=\"${option}_value\" {
                            set j 0
                            foreach enum_option $enum_list {
                                if { $j == $value } {
                                    set selected {selected="true"}
                                } else {
                                    set selected ""
                                }
                                cgi_option $enum_option value=$j $selected
                                incr j
                            }
                        }
                    }
                }
                FLOAT {
                    set format "%.02f"
                    table_data {
                        cgi_text ${option}_value=$value id=\"${option}_value\"
                    }
                }
                default {
                    table_data {
                        cgi_text ${option}_value=$value id=\"${option}_value\"
                    }
                }
            }
            td $unit
            table_data {
                button "Set" "onClick=SetValue('$interfaceId', '$channelAddress', '$parameterName', '${option}_value')"
            }
        }
        incr i
        puts ""
    }
}

proc cmd_calibrate {} {
    global env URL

    cgi_head {
        cgi_script src=\"../js/prototype.js\" ""
        title "Kalibrierung Busmodul"
    }
    
    body {
        h1 "Kalibrierung"
        set connections [xmlrpc $URL listValueConnections]
        
        table {
            table_row {bgcolor="#C0C0C0"} {
                th Channel
                th Parameter
                th Value
                th Unit
                th Action
            }
        
            foreach connection_info $connections {
                array_clear connection
                array set connection $connection_info
                if { [info exist connection(XmlRpcDevice)] } {
                    show_options connection
                }
            }
        }
    }
    cgi_javascript {
        puts "var url = \"$env(SCRIPT_NAME)\";"
        puts {
            dlgResult = 0;
            SetValue = function(iface, channel, id, input_id) {
                var input=document.getElementById(input_id);
                var value;
                if(input){
                    if(input.type=="checkbox")value=input.checked;
                    else value=input.value;
                }else value=input_id;
                
                var pb = "cmd=set_value";
                pb += "&iface="+iface;
                pb += "&channel="+channel;
                pb += "&valueId="+id;
                pb += "&value="+value;
                
                var opts = {
                    postBody: pb,
                    sendXML: false,
                    onSuccess: function(transport) {
                        if (transport.responseText.match(/^Success/g))PopupClose();
                        else alert("Wert konnte nicht gesetzt werden:\n"+transport.responseText);
                    }
                };
                new Ajax.Request(url, opts);
            }
        }
    
    }
}

proc cmd_set_value {} {
    global env URL TYPE_MAP
    import iface
    import channel
    import valueId
    import value
    
    set interface_list [xmlrpc $URL getInterfaces]
    foreach ifaceDescrList $interface_list {
        array_clear iface_descr
        array set ifaceDescr $ifaceDescrList
        if { "$ifaceDescr(Id)" == "$iface" } {
            set ifaceUrl $ifaceDescr(Url)
            break
        }
    }
    
    if { ![info exist ifaceUrl] } {
        puts "Unknown interface $iface"
        return
    }
    
    array set paramsetDescr [xmlrpc $ifaceUrl getParamsetDescription $channel MASTER]
    
    set struct ""
    
    array set parameterDescr $paramsetDescr($valueId)
    
    set sentry ""
    lappend sentry $valueId
    set type $parameterDescr(TYPE)
    lappend sentry [list $TYPE_MAP($type) $value]
    lappend struct $sentry

    xmlrpc $ifaceUrl putParamset [list string $channel] [list string MASTER] [list struct $struct]
    
    puts "Success"

}

cgi_eval {
if [file exists /etc/config/cgi_debug] {cgi_debug -on}
cgi_input

set cmd calibrate
catch { import cmd }
cmd_$cmd
}

