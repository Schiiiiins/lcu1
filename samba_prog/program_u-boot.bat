copy "..\bootloader\at91bootstrap\binaries\at91sam9261ek-dataflashboot-2.10.bin" .\at91bootstrap.bin
copy "..\bootloader\u-boot\u-boot.bin"	.

"C:\Programme\ATMEL Corporation\AT91-ISP v1.13\sam-ba 2.9\sam-ba.exe" \usb\ARM0 elv-lcu program.tcl bootstrap u-boot > logfile.log 2>&1
type logfile.log
pause
