copy "..\linux-2.6\linux-2.6\arch\arm\boot\uImage" .

sam-ba.exe \usb\ARM0 elv-lcu program.tcl kernel > logfile.log 2>&1
type logfile.log
pause
