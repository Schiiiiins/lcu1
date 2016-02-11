# ----------------------------------------------------------------------------
#         ATMEL Microcontroller Software Support 
# ----------------------------------------------------------------------------
# Copyright (c) 2008, Atmel Corporation
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice,
# this list of conditions and the disclaimer below.
#
# Atmel's name may not be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------


################################################################################
#  proc uboot_env: Convert u-boot variables in a string ready to be flashed
#                  in the region reserved for environment variables
################################################################################
proc set_uboot_env {nameOfLstOfVar} {
    upvar $nameOfLstOfVar lstOfVar
    
    # sector size is the size defined in u-boot CFG_ENV_SIZE
    set sectorSize [expr 0x4200 - 4]

    set strEnv [join $lstOfVar "\0"]
    while {[string length $strEnv] < $sectorSize} {
        append strEnv "\0"
    }
    set strCrc [binary format i [::vfs::crc $strEnv]]
    return "$strCrc$strEnv"
}

proc check_option { opt } {
    global argv
    if { [lsearch -exact $argv "$opt"] >= 0 } {
        return 1
    }
    if { [lsearch -exact $argv "all"] >= 0 } {
        return 1
    }
    return 0
}
################################################################################
#  Main script: Load the linux demo in DataFlash,
#               Update the environment variables
################################################################################
set bootstrapFile	"at91bootstrap.bin"
set ubootFile		"u-boot.bin"	
set kernelFile		"uImage"
set rootfsFile      "jffs2.image"

#set rootfsFile		"Angstrom-x11-image-demo-glibc-at91.rootfs.jffs2"

set ubootEnvFile	"ubootEnvtFileDataFlash.bin"

set MaxKernelSize   0x210000

## DataFlash Mapping
set baseAddr		0xC0000000
set bootStrapAddr	0x00000000
set ubootAddr		0x00008400
set ubootEnvAddr    0x00004200
set kernelAddr		0x00042000

## NandFlash Mapping
set rootfsAddr	0x00000000	 

# u-boot variable
set kernelUbootAddr [format "0x%08X" [expr $baseAddr + $kernelAddr]]
set kernelLoadAddr	0x22200000

# network settings
set nfsServer 192.168.1.3
set nfsRoot /nfsroot_lcu
set ipAddress 192.168.1.179
set gateway 192.168.1.1


set mtdparts "spi0.0-AT45DB642x:${kernelAddr}(u-boot),${MaxKernelSize}(kernel),-(free)"

lappend u_boot_variables \
    "ethaddr=3a:1f:34:08:54:54" \
    "bootdelay=1" \
    "baudrate=115200" \
    "stdin=serial" \
    "stdout=serial" \
    "stderr=serial" \
    "bootargs_flash=mem=64M console=ttyS0,115200 root=/dev/mtdblock0 ro rootfstype=jffs2 mtdparts=$mtdparts" \
    "bootcmd_flash=setenv bootargs \$(bootargs_flash); cp.b $kernelUbootAddr $kernelLoadAddr $MaxKernelSize; bootm $kernelLoadAddr"\
    "bootargs_net=mem=64M console=ttyS0,115200 root=/dev/nfs rw nfsroot=${nfsServer}:$nfsRoot ip=${ipAddress}::$gateway mtdparts=$mtdparts" \
    "bootcmd_net=setenv bootargs \$(bootargs_net); bootp; bootm \$(fileaddr)"\
    "bootcmd_usb=usb start; fatload usb 0:1 0x21000000 bootscript.img; source 0x21000000"\
    "bootcmd=run bootcmd_flash"\
    "updatecmd=run bootcmd_usb"\


puts "-I- === Args: $argv ==="

puts "-I- === Initialize the DataFlash access ==="
DATAFLASH::SelectDataflash AT91C_SPI0_CS0

if [check_option "erase"] {
    puts "-I- === Erase all the DataFlash blocs and test the erasing ==="
    DATAFLASH::EraseAllDataFlash
}

if [check_option "bootstrap" ] {
    puts "-I- === Load the bootstrap: dataflash_at91sam9263ek in the first sector ==="
    DATAFLASH::SendBootFile $bootstrapFile
}

if [check_option "u-boot" ] {
    puts "-I- === Load the u-boot in the next sectors ==="
    send_file {DataFlash AT45DB/DCB} "$ubootFile" $ubootAddr 0 
}

if [check_option "environment" ] {
    puts "-I- === Load the u-boot environment variables ==="
    set fh [open "$ubootEnvFile" w]
    fconfigure $fh -translation binary
    puts -nonewline $fh [set_uboot_env u_boot_variables]
    close $fh
    send_file {DataFlash AT45DB/DCB} "$ubootEnvFile" $ubootEnvAddr 0 
}

if [check_option "kernel" ] {
    puts "-I- === Load the Kernel image ==="
    send_file {DataFlash AT45DB/DCB} "$kernelFile" $kernelAddr 0
}

if [check_option "root-fs" ] {
    puts "-I- === Initialize the NAND access ==="
    NANDFLASH::Init

    puts "-I- === Erase all the NAND flash blocs and test the erasing ==="
    NANDFLASH::EraseAllNandFlash

    puts "-I- === Load the linux file system ==="
    send_file {NandFlash} "$rootfsFile" $rootfsAddr 0
}
