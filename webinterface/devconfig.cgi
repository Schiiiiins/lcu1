#!/bin/tclsh
source cgi.tcl
source common.tcl

set ICSERVER_URL "bin://127.0.0.1:2005"

set FLAGS(DONTDELETE) 8

proc list_available_devices {} {
    global env ICSERVER_URL
    set interface_list [xmlrpc $ICSERVER_URL getInterfaces]
    set devices ""
    foreach iface $interface_list {
        array_clear iface_descr
        array set iface_descr $iface
        catch {
            set url $iface_descr(Url)
            set devlist [xmlrpc $url listDevices]
            foreach dev $devlist {
                array_clear dev_descr
                array set dev_descr $dev
                if { "$dev_descr(PARENT)" == "" } {
                    set entry [list $dev_descr(ADDRESS) $iface_descr(Id) $dev_descr(TYPE)]
                    lappend devices $entry
                }
            }
        }
    }
    return $devices
}

set url ""
set iface ""

array set interfaces ""
array set interface_descriptions ""

set ICSERVER_URL "bin://127.0.0.1:2005"

proc read_interfaces {} {
    global env ICSERVER_URL interfaces interface_descriptions
    set interface_list [xmlrpc $ICSERVER_URL getInterfaces]
    foreach iface $interface_list {
        array_clear iface_descr
        array set iface_descr $iface
        catch {
            set url $iface_descr(Url)
            set id $iface_descr(Id)
            array set interfaces [list $id $url]
            array set interface_descriptions [list $id $id]
        }
    }
    return 1
}

proc put_javascript {} {
    global url env
    cgi_javascript "language=\"javascript\"" {
        puts "    var request = false;"
        puts "    function set_value(iface, device, id, type, input_id)"
        puts "    { "
        puts "      var input=document.getElementById(input_id);"
        puts "      var value;"
        puts "      if(input){"
        puts "          if(input.type==\"checkbox\")value=input.checked;"
        puts "          else value=input.value;"
        puts "      }else value=input_id;"
        puts ""        
        puts "      request = false;"
        puts "      if ( window.XMLHttpRequest ){ "
        puts "        try{ "
        puts "          request = new XMLHttpRequest();"
        puts "        } catch ( e ) {}"
        puts "      } else { "
        puts "        if ( window.ActiveXObject ){ "
        puts "          try{ "
        puts "            request = new ActiveXObject( \"Msxml2.XMLHTTP\" );"
        puts "          } catch ( e ) { "
        puts "            try{"
        puts "              request = new ActiveXObject(\"Microsoft.XMLHTTP\");"
        puts "            } catch ( e ) {}"
        puts "          } "
        puts "        } "
        puts "      }"
        puts "     "
        puts "      if ( request ){"
        puts "        request.open( \"GET\", \"$env(SCRIPT_NAME)?cmd=set_value&device=\"+device+\"&iface=\"+iface+\"&id=\"+id+\"&type=\"+type+\"&value=\"+value+\"\", true );"
        puts "        request.send(null);" 
        puts "        return( true );"
        puts "      }"
        puts "      return( false );"
        puts "    };"
        
        
        puts "    function activate_link_paramset(iface, device, ps_id, longpress)"
        puts "    { "
        puts "      request = false;"
        puts "      if ( window.XMLHttpRequest ){ "
        puts "        try{ "
        puts "          request = new XMLHttpRequest();"
        puts "        } catch ( e ) {}"
        puts "      } else { "
        puts "        if ( window.ActiveXObject ){ "
        puts "          try{ "
        puts "            request = new ActiveXObject( \"Msxml2.XMLHTTP\" );"
        puts "          } catch ( e ) { "
        puts "            try{"
        puts "              request = new ActiveXObject(\"Microsoft.XMLHTTP\");"
        puts "            } catch ( e ) {}"
        puts "          } "
        puts "        } "
        puts "      }"
        puts "     "
        puts "      if ( request ){"
        puts "        request.open( \"GET\", \"$env(SCRIPT_NAME)?cmd=activate_link_paramset&device=\"+device+\"&iface=\"+iface+\"&ps_id=\"+ps_id+\"&long=\"+longpress, true );"
        puts "        request.send(null);" 
        puts "        return( true );"
        puts "      }"
        puts "      return( false );"
        puts "    };"
    }
}

proc cmd_main {} {
    global env
    title "LCU1 - XML-RPC Gerätekonfiguration"
    body {
        h2 "XML-RPC Gerätekonfiguration"
        h3 [url "Geräteliste" $env(SCRIPT_NAME)?cmd=list_devices]
        h3 [url "Kanalliste" $env(SCRIPT_NAME)?cmd=list_channels]
        h3 [url "Liste der Interfaceprozesse" $env(SCRIPT_NAME)?cmd=list_interfaces]
        h3 [url "Übersicht Gerätestatus" $env(SCRIPT_NAME)?cmd=device_status]
    }
}

proc get_devicenames {arr_var} {
    upvar $arr_var arr
    array_clear arr
    catch {
        array set result [rega_script {
            string dev_id;
            string devs;
            foreach(dev_id, dom.GetObject(ID_DEVICES).EnumUsedIDs()){
                object dev=dom.GetObject(dev_id);
                if( (dev.Interface() != 65535) && (dev.Address() != "") && dev.CreatingCompleted() ) {
                    devs = devs # " " # dev.Address() # " {" # dev.Name() # "} ";
                }
            }
        }]
        array set arr $result(devs)
    }
}

proc cmd_set_value {} {
    global url env
    import device
    import id
    import type
    import value
	#exec echo xmlrpc $url setValue $device $id $type $value >> /var/tmp/out.txt
    xmlrpc $url setValue [list string $device] [list string $id] [list $type $value ]
}

proc cmd_list_channels {} {
    global env interfaces FLAGS
    title "LCU1 - Kanalliste"
    body {
    	h2 "Kanalliste"
        foreach iface [array names interfaces] {
		    h3 "Interfaceprozess: $iface"
            if { [ catch {
        	    table { 
                    set url $interfaces($iface)
                    set devlist [xmlrpc $url listDevices [list bool 0]]
                    foreach dev $devlist {
                        array_clear dev_descr
                        array set dev_descr $dev
                        if { "$dev_descr(PARENT)" != "" } {
                            if { "$dev_descr(TYPE)" == "VIRTUAL_KEY" } continue;
                            puts ""
                            table_row { 
                                table_data {
                                    h3 [url "$dev_descr(TYPE) ($dev_descr(ADDRESS))" $env(SCRIPT_NAME)?cmd=config&device=$dev_descr(ADDRESS)&iface=$iface]
                                }
                                if { !($dev_descr(FLAGS) & $FLAGS(DONTDELETE)) } {
                                    table_data {
                                        h3 [url "Gerät löschen" $env(SCRIPT_NAME)?cmd=delete&device=$dev_descr(PARENT)&iface=$iface]
                                    }
                                    table_data {
                                        h3 [url "Gerät löschen und zurücksetzen" $env(SCRIPT_NAME)?cmd=delete&device=$dev_descr(PARENT)&reset=1&iface=$iface]
                                    }
                                } else {
                                    td colspan=2 ""
                                }
                            }
                        }
                    }
                } 
            } ] } { puts "Interfaceprozess $iface nicht ansprechbar" }
			hr
        }
        puts ""
    }
}

proc cmd_list_devices {} {
    global env interfaces FLAGS
    set expand ""
    catch { import expand }
    title "LCU1 - Geräteliste"
    body {
    	h2 "Geräteliste"
   	    table { 
            foreach iface [array names interfaces] {
                if { [ catch {
                    set url $interfaces($iface)
                    set devlist [xmlrpc $url listDevices [list bool 0]]
                    foreach dev $devlist {
                        array_clear dev_descr
                        array set dev_descr $dev
                        if { "$dev_descr(PARENT)" == "" } {
                            puts ""
                            table_row { 
                                table_data colspan=2 {
                                    h3 [url "$dev_descr(TYPE) ($dev_descr(ADDRESS))" $env(SCRIPT_NAME)?cmd=list_devices&expand=$dev_descr(ADDRESS)]
                                }
                                if { !($dev_descr(FLAGS) & $FLAGS(DONTDELETE)) } {
                                    table_data {
                                        h3 [url "Gerät löschen" $env(SCRIPT_NAME)?cmd=delete&device=$dev_descr(ADDRESS)&iface=$iface]
                                    }
                                    table_data {
                                        h3 [url "Gerät löschen und zurücksetzen" $env(SCRIPT_NAME)?cmd=delete&device=$dev_descr(ADDRESS)&reset=1&iface=$iface]
                                    }
                                } else {
                                    td colspan=2 ""
                                }
                                table_data {
                                    if { [info exist dev_descr(AVAILABLE_FIRMWARE)] } {
                                        h3 [url "Firmwareupdate ($dev_descr(FIRMWARE) auf $dev_descr(AVAILABLE_FIRMWARE))" $env(SCRIPT_NAME)?cmd=firmware_update&device=$dev_descr(ADDRESS)&iface=$iface]
                                    }
                                }
                            }
                        } elseif { "$dev_descr(PARENT)" == "$expand" } {
                            puts ""
                            table_row { 
                                table_data width=10 { }
                                table_data {
                                    h3 [url "$dev_descr(TYPE) ($dev_descr(ADDRESS))" $env(SCRIPT_NAME)?cmd=config&device=$dev_descr(ADDRESS)&iface=$iface]
                                }
                                table_data colspan=3 { }
                            }
                        }
                    } 
                }] } { table_data colspan=4 { puts "Interfaceprozess $iface nicht ansprechbar" } }
            }
        }
        puts ""
    }
}

array set MAINTENANCE_VALUES { CONFIG_PENDING flag LOWBAT flag STICKY_UNREACH flag UNREACH flag AES_KEY value}
proc cmd_device_status {} {
    global env interfaces MAINTENANCE_VALUES
    title "LCU1 - Übersicht Gerätestatus"
    body {
    	h2 "Übersicht Gerätestatus"
   	    table { 
            table_row {
                th ""
                foreach name [array names MAINTENANCE_VALUES] {
                    th $name
                }
            }
            foreach iface [array names interfaces] {
                #catch {
                    set url $interfaces($iface)
                    set devlist [xmlrpc $url listDevices [list bool 0]]
                    foreach dev $devlist {
                        array_clear dev_descr
                        array set dev_descr $dev
                        if { "$dev_descr(PARENT)" != "" && "$dev_descr(TYPE)" == "MAINTENANCE" } {
                            puts ""
                            table_row { 
                                table_data {
                                    h3 "$dev_descr(PARENT)"
                                }
                                array set valueset [xmlrpc $url getParamset [list string $dev_descr(ADDRESS)] [list string VALUES]]
                                foreach name [array names MAINTENANCE_VALUES] {
                                    set type $MAINTENANCE_VALUES($name)
                                    table_data {
                                        if { [info exist valueset($name)] } {
                                            if { "$type" == "flag" } {
                                                if { $valueset($name) } {
                                                    h3 $name
                                                }
                                            } elseif { "$type" == "value" } {
                                                puts "$valueset($name)"
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    } 
                #}
            }
        }
        puts ""
    }
}

proc cmd_list_interfaces {} {
    global env interfaces interface_descriptions
    title "LCU1 - Liste der Interfaceprozesse"
    body {
    	h2 "Liste der Interfaceprozesse"
        foreach iface [array names interfaces] {
            set url $interfaces($iface)
            set description $interface_descriptions($iface)
            h3 "$iface ($description)"
            puts ""
            if [ catch {
                catch {
                    #check if the interface supports install mode
                    #failure of this call will throw us out of here
                    xmlrpc $url system.methodHelp setInstallMode
                    if { [xmlrpc $url getInstallMode ""] } {
                        puts "Installationsmodus ist aktiv"
                        puts [url "Installationsmodus beenden" $env(SCRIPT_NAME)?cmd=interface_cmd&iface=$iface&method=setInstallMode&args=bool%20false]
                    } else {
                        puts "Installationsmodus ist nicht aktiv"
                        puts [url "Installationsmodus starten" $env(SCRIPT_NAME)?cmd=interface_cmd&iface=$iface&method=setInstallMode&args=bool%20true]
                    }
                    br
                }
                catch {
                    #check if the interface supports discovery
                    #failure of this call will throw us out of here
                    xmlrpc $url system.methodHelp searchDevices
                    puts [url "Geräte suchen" $env(SCRIPT_NAME)?cmd=interface_cmd&iface=$iface&method=searchDevices&args=]
                    br
                }
                catch {
                    #check if the interface supports discovery
                    #failure of this call will throw us out of here
                    xmlrpc $url system.methodHelp addDevice
                    form $env(SCRIPT_NAME) method=get {
                        export cmd=interface_cmd
                        export iface=$iface
                        export method=addDevice
                        puts -nonewline "Serial# "
                        cgi_text "args="
                        submit_button "submit=Gerät hinzufügen"
                    }
                    br
                }
                catch {
                    #check if the interface supports key exchange
                    #failure of this call will throw us out of here
                    xmlrpc $url system.methodHelp changeKey
                    form $env(SCRIPT_NAME) method=get {
                        export cmd=interface_cmd
                        export iface=$iface
                        export method=changeKey
                        puts -nonewline "AES-Schlüssel"
                        cgi_text "args="
                        submit_button "submit=Schlüssel ändern"
                    }
                    form $env(SCRIPT_NAME) method=get {
                        export cmd=interface_cmd
                        export iface=$iface
                        export method=setTempKey
                        puts -nonewline "AES-Schlüssel"
                        cgi_text "args="
                        submit_button "submit=Temporären Schlüssel setzen"
                    }
                }
            } ] { puts "Interfaceprozess $iface nicht ansprechbar" }
            br
        }
    }
}

proc cmd_interface_cmd {} {
    global env url iface
    import method
    import args
    xmlrpc $url $method $args
    redirect $env(SCRIPT_NAME)?cmd=list_interfaces
}

proc cmd_put_paramset {} {
    global url iface env TYPE_MAP
    import ps_id
    import ps_type
    import device
    import redir_url
    array set ps_descr [xmlrpc $url getParamsetDescription [list string $device] [list string $ps_type]]
    set struct ""
    foreach param_id [lsort [array names ps_descr]] {
        array_clear param_descr
        array set param_descr $ps_descr($param_id)
        set sentry ""
        lappend sentry $param_id
        set type $param_descr(TYPE)
        set value 0
        catch { import_as $param_id value }
        lappend sentry [list $TYPE_MAP($type) $value]
        lappend struct $sentry
    }
    xmlrpc $url putParamset [list string $device] [list string $ps_id] [list struct $struct]

	#Rudiment???
    array set ps [xmlrpc $url getParamset [list string $device] [list string $ps_id]]

    redirect $redir_url
}


proc show_paramset {device ps_type ps_id ps_var ps_descr_var redir_url} {
    global iface env TYPE_MAP url
    upvar $ps_var ps
    upvar $ps_descr_var ps_descr
    form $env(SCRIPT_NAME) method=get {
        export cmd=put_paramset
        export ps_id
        export ps_type
        export device
        export iface
        export redir_url
        table {
            foreach param_id [lsort [array names ps_descr]] {
                array_clear param_descr
                array set param_descr $ps_descr($param_id)
                set type $param_descr(TYPE)
                set unit $param_descr(UNIT)
                set min $param_descr(MIN)
                set max $param_descr(MAX)
                set operations $param_descr(OPERATIONS)
                set id "id=\"$ps_id.$param_id\""
                set value ""
                if { ! ($operations & 3) } continue
                set unknown ""
                if { $operations & 1 } {
                    if [info exist ps($param_id)] {
                        set value $ps($param_id) 
                    } else {
                        set unknown " (unknown)"
                        set value $param_descr(DEFAULT)
                    }
                }
                if { $operations & 2 } {
                    set access ""
                } else {
                    set access "readonly=\"true\""
                }
                puts ""
                table_row {
                    td "$param_id$unknown"
                    switch $type {
                        "BOOL" {
                            if { $value } {
                                set checked "checked"
                            } else {
                                set checked ""
                            }
                            table_data { cgi_checkbox $param_id=1 $id $access $checked}
                            td
                        }
                        "STRING" {
                            table_data { cgi_text $param_id=$value $id $access }
                            td "$unit"
                        }
                        "INTEGER" {
                            table_data {
                                set hidden ""
                                if { [ info exist param_descr(SPECIAL) ] } {
                                    cgi_select _$param_id size=1 "onchange=\"document.getElementById('$ps_id.$param_id').value=this.options\[this.selectedIndex\].value;document.getElementById('$ps_id.$param_id').style.visibility=(this.selectedIndex < [llength $param_descr(SPECIAL)])?'hidden':'visible';\"" {
                                        set i 0
                                        foreach _sv $param_descr(SPECIAL) {
                                            array set sv $_sv
                                            if { $sv(VALUE) == $value } {
                                                set selected selected
                                            } else  {
                                                set selected ""
                                            }
                                            cgi_option $sv(ID) "value=$sv(VALUE)" $selected
                                            incr i
                                        }
                                        if { $value >= $param_descr(MIN) && $value <= $param_descr(MAX) } {
                                            set selected selected
                                            set hidden ""
                                        } else {
                                            set selected ""
                                            set hidden "style=\"visibility:hidden;\""
                                        }
                                        set default_value $param_descr(DEFAULT)
                                        if { $default_value < $param_descr(MIN) || $default_value > $param_descr(MAX) } {
                                            set default_value $param_descr(MIN)
                                        }
                                        cgi_option "Wert eingeben" "value=$default_value" $selected
                                    }
                                }
                                cgi_text $param_id=$value $id $access $hidden
                            }
                            td "$unit ($min-$max)"
                        }
                        "FLOAT" {
                            table_data {
                                set hidden ""
                                if { [ info exist param_descr(SPECIAL) ] } {
                                    cgi_select _$param_id size=1 "onchange=\"document.getElementById('$ps_id.$param_id').value=this.options\[this.selectedIndex\].value;document.getElementById('$ps_id.$param_id').style.visibility=(this.selectedIndex < [llength $param_descr(SPECIAL)])?'hidden':'visible';\"" {
                                        set i 0
                                        foreach _sv $param_descr(SPECIAL) {
                                            array set sv $_sv
                                            if { $sv(VALUE) == $value } {
                                                set selected selected
                                            } else  {
                                                set selected ""
                                            }
                                            cgi_option $sv(ID) "value=$sv(VALUE)" $selected
                                            incr i
                                        }
                                        if { $value >= $param_descr(MIN) && $value <= $param_descr(MAX) } {
                                            set selected selected
                                            set hidden ""
                                        } else {
                                            set selected ""
                                            set hidden "style=\"visibility:hidden;\""
                                        }
                                        set default_value $param_descr(DEFAULT)
                                        if { $default_value < $param_descr(MIN) || $default_value > $param_descr(MAX) } {
                                            set default_value $param_descr(MIN)
                                        }
                                        cgi_option "Wert eingeben" "value=$default_value" $selected
                                    }
                                }
                                cgi_text $param_id=$value $id $access $hidden
                            }
                            td "$unit ($min-$max)"
                        }
                        "ENUM" {
                            table_data {
                                set value_list $param_descr(VALUE_LIST)
                                if { "$access" == "" } {
                                    cgi_select $param_id size=1 $id {
                                        set i 0
                                            foreach v $value_list {
                                                if { ![string length $v] } continue
                                                if { $i == $value } {
                                                    cgi_option $v value=$i selected
                                                } else  {
                                                    cgi_option $v value=$i
                                                }
                                            incr i
                                        }
                                    }
                                } else {
                                    puts "[lindex $value_list $value]"
                                }
                            }
                            td
                        }
                        "ACTION" {
                            table_data { cgi_checkbox $param_id=1 $id $access "style=\"visibility:hidden;\"" }
                            td
                        }
                        default {
                            puts "Unbekannter Typ \"$type\"<br>"
                        }
                    }
                    if { $ps_type == "VALUES" } {
                        table_data {
                            if { $operations & 2 } {
                                button "Setzen" "onClick=set_value('$iface', '$device', '$param_id', '$TYPE_MAP($type)', '$ps_id.$param_id')"
                            }
                        }
                    }
                    if { $operations & 8 } {
                        td [url "Wert ermitteln" $env(SCRIPT_NAME)?cmd=determine_parameter&iface=$iface&device=$device&ps_id=$ps_id&param_id=$param_id]
                    }
                }
                puts ""
            }
        }
        if { $ps_type != "VALUES" } {
            submit_button "submit=Änderungen abschicken"
            puts ""
        }
        if { $ps_type == "LINK" } {
            button "Kurzen Tastendruck simulieren" "onClick=activate_link_paramset('$iface', '$device', '$ps_id', false)"
            puts ""
            button "Langen Tastendruck simulieren" "onClick=activate_link_paramset('$iface', '$device', '$ps_id', true)"
            puts ""
        }
    }
}

proc cmd_determine_parameter {} {
    global iface url env
    import device
    import ps_id
    import param_id
    xmlrpc $url determineParameter [list string $device] [list string $ps_id] [list string $param_id]
    redirect $env(SCRIPT_NAME)?cmd=config&iface=$iface&device=$device
}

proc cmd_clear_config_cache {} {
    global iface url env
    import device
    xmlrpc $url clearConfigCache [list string $device]
    redirect $env(SCRIPT_NAME)?cmd=config&iface=$iface&device=$device
}

proc cmd_delete {} {
    global url iface env
    import device
    set reset 0
    catch { import reset }
    xmlrpc $url deleteDevice [list string $device] [list boolean $reset]
    redirect $env(SCRIPT_NAME)?cmd=list_devices&iface=$iface
}

proc cmd_firmware_update {} {
    global url iface env
    import device
    set channel ""
    catch {import channel}
    set result [xmlrpc $url updateFirmware [list string $device]]
    if { [lindex $result 0] } {
        set success "erfolgreich"
    } else {
        set success "fehlgeschlagen"
    }
    title "Firmware update für $device $success"
    body {
        h2 "Firmware update für $device $success"
        if { "$channel" != "" } {
            puts [url "Zurück zur Kanalkonfiguration" $env(SCRIPT_NAME)?cmd=config&iface=$iface&device=$channel]
        } else {
            puts [url "Zurück zur Geräteliste" $env(SCRIPT_NAME)?cmd=list_devices]
        }
    }
}

proc cmd_config {} {
    global url iface env
    import device
    title "LCU1 - Kanalkonfiguration $device"
    put_javascript
    body {
        array set dev_descr [xmlrpc $url getDeviceDescription [list string $device]]
    	h2 "Kanalkonfiguration für $dev_descr(TYPE) $device"
        set parent $dev_descr(PARENT)
        if { $parent != "" } {
            array set parent_descr [xmlrpc $url getDeviceDescription [list string $parent]]
            puts "Firmwareversion $parent_descr(FIRMWARE)\n"
            if { [info exist parent_descr(AVAILABLE_FIRMWARE)] } {
                br
                puts "Verfügbare Firmwareversion $parent_descr(AVAILABLE_FIRMWARE)\n"
            }
            if { [lsearch $parent_descr(PARAMSETS) "MASTER"] >= 0 } {
                array set parent_ps_descr [xmlrpc $url getParamsetDescription [list string $parent] [list string "MASTER"]]
                array set parent_ps [xmlrpc $url getParamset [list string $parent] [list string "MASTER"]]
                if { [llength [array names parent_ps_descr]] } {
                    h3 "Geräteparameter"
                    puts ""
                    show_paramset $parent "MASTER" "MASTER" parent_ps parent_ps_descr $env(SCRIPT_NAME)?cmd=config&iface=$iface&device=$device
                }
            }
            h3 [url "Firmwareupdate" $env(SCRIPT_NAME)?cmd=firmware_update&device=$parent&channel=$device&iface=$iface]
        }
        foreach ps_id $dev_descr(PARAMSETS) {
            array_clear ps_descr
            array set ps_descr [xmlrpc $url getParamsetDescription [list string $device] [list string $ps_id]]
            if { "$ps_id" == "MASTER" } {
                array_clear ps
                array set ps [xmlrpc $url getParamset [list string $device] [list string $ps_id]]
                if { [llength [array names ps]] } {
                    if { "$dev_descr(PARENT)" == "" } {
                        h3 "Geräteparameter"
                        puts ""
                        br
                    } else {
                        h3 "Kanalparameter"
                        puts ""
                    }
                    show_paramset $device $ps_id $ps_id ps ps_descr $env(SCRIPT_NAME)?cmd=config&iface=$iface&device=$device
                }
            }
            if { "$ps_id" == "VALUES" } {
                if { [llength [array names ps_descr]] } {
                    h3 "Werte"
                    puts ""
                    array_clear ps
                    foreach id [array names ps_descr] {
                        catch {
                            set ps($id) [xmlrpc $url getValue [list string $device] [list string $id]]
                        }
                    }
                    show_paramset $device $ps_id $ps_id ps ps_descr $env(SCRIPT_NAME)?cmd=config&iface=$iface&device=$device
                }
            }
        }
    }
}

cgi_eval {
if [file exists /etc/config/cgi_debug] {cgi_debug -on}
cgi_input
catch {
    import debug
    cgi_debug -on
}
read_interfaces
catch {
    import iface
    set url $interfaces($iface)
}

set cmd main
catch { import cmd }
cmd_$cmd

}
