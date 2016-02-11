#!/bin/tclsh
source cgi.tcl
source common.tcl

proc cmd_upload {} {
    global env
    
    http_head

    cgi_head {
        title "LCU1 - Firmwareupdate Schritt 1"
    }
    
    body {
	h2 "Firmwareupdate Schritt 1"
	h3 "Wählen Sie die Firmwaredatei aus und klicken sie auf Übertragen, um sie in das RAM der LCU1 zu übertragen."
        form $env(SCRIPT_NAME) enctype=multipart/form-data method=post {
                export cmd=confirm
                file_button firmware_file size=50 maxlength=8000000
		br
		br
                submit_button "submit=Übertragen"
        }
    br
	puts [url "Firmwareupdate abbrechen und zurück zur Startseite" /]
    }
}

proc cmd_confirm {} {
    global env
    import_file -client firmware_file
    file rename -force -- [lindex $firmware_file 0] "/var/new_firmware.tar.gz"
    cgi_head {
        title "LCU1 - Firmwareupdate Schritt 2"
    }
    
    body {
	h2 "Firmwareupdate Schritt 2"
	h3 "Um die übertragene Firmware dauerhaft zu speichern, klicken Sie auf Flash."
	puts "<bold>Achtung: </bold>Wenn dieser Schritt fehlschlägt, z.B. durch eine Unterbrechnung der Spannungsversorgung, muss die LCU1 neu in Betrieb genommen werden!"
	br
	br
        form $env(SCRIPT_NAME) method=post {
                export cmd=flash
                submit_button "submit=Flash"
        }
	br
	puts [url "Firmwareupdate abbrechen und zurück zur Startseite" /]
    }
}

proc cmd_flash {} {
    cgi_head {
        title "LCU1 - Firmwareupdate Schritt 3"
    }
    
    body {
	h2 "Firmwareupdate Schritt 3"
	h3 "Die neue Firmware wird ins Flash geschrieben."
	puts "Dieser Schritt kann mehrere Minuten dauern."
    br
	puts "Sie können den folgenden Link verwenden, wenn die LCU1 wieder betriebsbereit ist."
	br
	puts [url "Zurück zur Startseite" /]
    }
    exec kill -quit 1
}

cgi_eval {
if [file exists /etc/config/cgi_debug] {cgi_debug -on}
cgi_input

set cmd upload
catch { import cmd }
cmd_$cmd
}

