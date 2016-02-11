copy "..\linux-2.6\linux-2.6\arch\arm\boot\uImage" .
copy "..\jffs2.image" jffs2.image

"C:\Programme\ATMEL Corporation\AT91-ISP v1.13\sam-ba 2.9\sam-ba.exe" \usb\ARM0 elv-lcu program.tcl root-fs > logfile.log 2>&1
type logfile.log
pause
