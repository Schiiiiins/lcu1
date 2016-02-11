#!/bin/tclsh
source cgi.tcl
source common.tcl

set ICSERVER_URL "bin://127.0.0.1:2005"

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

proc generate_js_objects { mapped_devices_array } {
    global env ICSERVER_URL
    upvar $mapped_devices_array mapped_devices
    
    cgi_javascript {
        set devlist [xmlrpc $ICSERVER_URL getDevices]
    
        puts {  availableDevices = new Array(); }
        set available_devices [list_available_devices]
        foreach dev $available_devices {
            set address [lindex $dev 0]
            set iface [lindex $dev 1]
            set type [lindex $dev 2]
            puts {  var device = new Object(); }
            puts "  device\['Address'\] = '$address';"
            puts "  device\['Interface'\] = '$iface';"
            puts "  device\['Type'\] = '$type';"
            if { [info exist mapped_devices($address)] } {
                puts "  device\['Mapped'\] = 1;"
            } else {
                puts "  device\['Mapped'\] = 0;"
            }
            puts "  availableDevices\[availableDevices.length\] = device;"
        }    
    }
}

proc action_put_page {} {
    global env ICSERVER_URL

    set devlist [xmlrpc $ICSERVER_URL getDevices]
    
    http_head

    cgi_head {
        cgi_script src=\"prototype.js\" ""
        title "LCU1 - Zuordnungen"
    }
    
    body topmargin="0" leftmargin="0" {
        h2 "Zuordnungen"
        table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" {
            puts ""
            table_row {
                td [bold "Logische Geräte - Zuordnungen"]
                td 
                td [bold "Physikalische Geräte"]
            }
            puts ""
            array_clear mapped_devices
            array_clear allowed_types
            table_row {
                table_data width=400 {
                    cgi_select mappings {id="list_mappings"} size=20 "style=width:100%" "onchange=OnMappingsSelectionChanged()" {
                        foreach dev $devlist {
                            array_clear dev_descr
                            array set dev_descr $dev
                            if { ! [info exist dev_descr(MappedAddress)] } {
                                cgi_option "$dev_descr(DisplayName) - nicht zugeordnet" "value=$dev_descr(Id)|$dev_descr(Type)||1"
                                set allowed_types($dev_descr(Type)) 1
                            } else {
                                cgi_option "$dev_descr(DisplayName) - $dev_descr(MappedAddress)" "value=$dev_descr(Id)|$dev_descr(Type)|$dev_descr(MappedAddress)|$dev_descr(MappedByUser)"
                                set mapped_devices($dev_descr(MappedAddress)) 1
                                set allowed_types($dev_descr(Type)) 1
                            }
                        }    
                    }
                }
                table_data width=10 {
                }
                table_data width=400 {
                    cgi_select mappings {id="list_devices"} size=20 style="width:100%" "onchange=UpdateButtonStates()" {
                    }
                }
            }
            table_row {
                table_data colspan=2 {
                    table {
                        table_row {
                            table_data {
                                cgi_button "Zuordnung löschen" id=button_unmap {onClick=OnUnmap()}
                            }
                            table_data {
                                cgi_button "Zuordnung herstellen" id=button_map {onClick=OnMap()}
                            }
                            table_data {
                                cgi_button "Geräte suchen" id=button_search {onClick=OnSearch()}
                            }
                        }
                    }
                }
            }
        }
    }
    generate_js_objects mapped_devices
        
    cgi_javascript {
        puts "var url = \"$env(SCRIPT_NAME)\";"
        
        puts {
            UpdateButtonStates = function() {
                var selectedMappingValue = $('list_mappings').value;
                if( selectedMappingValue == "" )
                {
                    $('button_unmap').disabled = true;
                    $('button_map').disabled = true;
                    return;
                }
                var mappedAddress = selectedMappingValue.split('|')[2];
                var mappedByUser = parseInt(selectedMappingValue.split('|')[3]);
                if( !mappedByUser )
                {
                    $('button_unmap').disabled = true;
                    $('button_map').disabled = true;
                    return;
                }
                if( mappedAddress != "" )
                {
                    $('button_unmap').disabled = false;
                    $('button_map').disabled = true;
                    return;
                }
                var selectedDeviceValue = $('list_devices').value;
                if( selectedDeviceValue == "" )
                {
                    $('button_unmap').disabled = true;
                    $('button_map').disabled = true;
                    return;
                }
                var deviceType = selectedDeviceValue.split('|')[2];
                var deviceAddress = selectedDeviceValue.split('|')[0];
                var deviceMapped = parseInt(selectedDeviceValue.split('|')[3]);
                if( deviceAddress == mappedAddress )
                {
                    $('button_unmap').disabled = false;
                    $('button_map').disabled = true;
                    return;
                }
                var mappingType = selectedMappingValue.split('|')[1];
                if( mappingType != deviceType || deviceMapped )
                {
                    $('button_unmap').disabled = true;
                    $('button_map').disabled = true;
                    return;
                }
                $('button_unmap').disabled = true;
                $('button_map').disabled = false;
            }
            
            FillAvailableList = function() {
                $('list_devices').length = 0;
                var selectedMappingValue = $('list_mappings').value;
                if( selectedMappingValue == "" )
                {
                    $('list_devices').disabled = true;
                }else{
                    $('list_devices').disabled = false;
                }
                var mappedType = selectedMappingValue.split('|')[1];
                var mappedAddress = selectedMappingValue.split('|')[2];
                for( i=0; i<availableDevices.length; i++ )
                {
                    if( availableDevices[i]['Type'] == mappedType )
                    {
                        entry = new Option(availableDevices[i]['Address'] + " - " + availableDevices[i]['Type'], availableDevices[i]['Address'] + "|" + availableDevices[i]['Interface'] + "|" + availableDevices[i]['Type'] + "|" + availableDevices[i]['Mapped']);
                    }else{
                        entry = new Option("( " + availableDevices[i]['Address'] + " - " + availableDevices[i]['Type'] + " )", availableDevices[i]['Address'] + "|" + availableDevices[i]['Interface'] + "|" + availableDevices[i]['Type'] + "|" + availableDevices[i]['Mapped']);
                    }
                    $('list_devices').options[$('list_devices').length] = entry;
                    if( availableDevices[i]['Address'] == mappedAddress )
                    {
                        entry.selected = true;
                        $('list_devices').disabled = true;
                    }
                }
            }
            
            
            OnMappingsSelectionChanged = function() {
                FillAvailableList();
                UpdateButtonStates();
            }
            
            OnDeviceSelectionChanged = function() {
                UpdateButtonStates();
            }
            
            OnMap = function() {
                var selectedMappingValue = $('list_mappings').value;
                if( selectedMappingValue == "" )
                {
                    return;
                }
                var mappedAddress = selectedMappingValue.split('|')[2];
                if( mappedAddress != "" )
                {
                    return;
                }
                var idToMap = selectedMappingValue.split('|')[0];
                var typeToMap = selectedMappingValue.split('|')[1];
                var selectedDeviceValue = $('list_devices').value;
                if( selectedDeviceValue == "" )
                {
                    return;
                }
                var deviceAddress = selectedDeviceValue.split('|')[0];
                var deviceInterface = selectedDeviceValue.split('|')[1];
                
                var pb = "action=map";
                pb += "&deviceId="+idToMap;
                pb += "&mapAddress="+deviceAddress;
                pb += "&mapInterface="+deviceInterface;
                
                var opts = {
                    postBody: pb,
                    sendXML: false,
                    onSuccess: function(transport) {
                        if (transport.responseText.match(/^Success/g))
                        {
                            document.location.href = document.location.href;
                        }
                        else alert("Zuordnung konnte nicht gespeichert werden:\n"+transport.responseText);
                    }
                };
                new Ajax.Request(url, opts);
            }
            
            OnUnmap = function() {
                var selectedMappingValue = $('list_mappings').value;
                if( selectedMappingValue == "" )
                {
                    return;
                }
                var mappedAddress = selectedMappingValue.split('|')[2];
                if( mappedAddress == "" )
                {
                    return;
                }
                var idToUnmap = selectedMappingValue.split('|')[0];
                
                var pb = "action=unmap";
                pb += "&deviceId="+idToUnmap;
                
                var opts = {
                    postBody: pb,
                    sendXML: false,
                    onSuccess: function(transport) {
                        if (transport.responseText.match(/^Success/g))
                        {
                            document.location.href = document.location.href;
                        }
                        else alert("Zuordnung konnte nicht gelöscht werden:\n"+transport.responseText);
                    }
                };
                new Ajax.Request(url, opts);
            }
            
            OnSearch = function() {
                var pb = "action=search";
                
                var opts = {
                    postBody: pb,
                    sendXML: false,
                    onSuccess: function(transport) {
                        document.location.href = document.location.href;
                    }
                };
                new Ajax.Request(url, opts);
            }
        }
        puts "FillAvailableList();UpdateButtonStates();"       
    }
}

proc action_map {} {
    global env ICSERVER_URL

    catch {
        import deviceId
        import mapAddress
        import mapInterface

        xmlrpc $ICSERVER_URL mapDevice $deviceId $mapInterface $mapAddress
        puts "Success"
        return
    } errMsg
    puts "$errMsg"
}

proc action_unmap {} {
    global env ICSERVER_URL

    catch {
        import deviceId

        xmlrpc $ICSERVER_URL unmapDevice $deviceId
        puts "Success"
        return
    } errMsg
    puts "$errMsg"
}

proc action_search {} {
    global env ICSERVER_URL
    http_head
    set interface_list [xmlrpc $ICSERVER_URL getInterfaces]
    puts "Interfaces: $interface_list"
    foreach iface $interface_list {
        puts "Interface: $iface"
        array set iface_descr $iface
        set error ""
        catch {
            set url $iface_descr(Url)
            puts "$url: [xmlrpc $url searchDevices]"
        } error
        if { [string length $error] }  { puts $error }
    }
}

cgi_eval {
    if [file exists /etc/config/cgi_debug] {cgi_debug -on}
    cgi_input
    catch {
        import debug
        cgi_debug -on
    }
    set action "put_page"

    catch { import action }
    action_$action
}


