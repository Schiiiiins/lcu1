#!/usr/bin/tclsh

package require tdom

proc putValue { fd node prefix } {
    set name [$node getAttribute "name"]
    set alias [$node getAttribute "alias"]
    set ignore [$node getAttribute "iccignore" false]
    set unit [$node getAttribute "unit" ""]
    set default_value [$node getAttribute "default_value" ""]
    set description [$node getAttribute "description" ""]
    if { "$ignore" == "true" } return
    set comment ""
    if { [string length $unit] } { set comment "$comment unit = $unit" }
    if { [string length $default_value] } { set comment "$comment default = $default_value" }
    if { [string length $description] } { set comment "$comment $description" }
    if { [string length $comment] } { set comment " /*$comment */" }
    
    if { "[string index $name 1]" == "X" } {
        set datatype "bit"
    } else {
        set datatype "int"
    }
    if { "[string index $name 0]" == "Q" } {
        puts $fd "${prefix}imm $datatype $alias; $name = $alias;$comment"
    }
    if { "[string index $name 0]" == "I" } {
        puts $fd "${prefix}imm $datatype $alias = $name;$comment"
    }
}

proc putLoopStart { fd node } {
    set startIndex [$node getAttribute "start"]
    set count [$node getAttribute "count"]
    if { $startIndex } {
        puts $fd "FOR (n = $startIndex; n <= [expr $startIndex + $count - 1]; n++) \{"
    } else {
        puts $fd "FOR (n = 0; n < [expr $count]; n++) \{"
    }
}

proc putLoopEnd { fd } {
    puts $fd "\}"
    puts $fd ""
}



set filename [lindex $argv 0]
set fd [open $filename r]
if { $fd <0 } { 
    puts "Error opening file $filename"
    exit
}

set xml [read $fd]

close $fd

set outfilename [lindex $argv 1]
set fd [open $outfilename w]
if { $fd <0 } { 
    puts "Error opening file $outfilename"
    exit
}

set doc [dom parse $xml]
set root [$doc documentElement]

foreach node [$root childNodes] {
    if { "[$node nodeName]" == "values" } {
        foreach valuesChild [$node childNodes] {
            if { "[$valuesChild nodeName]" == "value" } {
                putValue $fd $valuesChild ""
            } elseif { "[$valuesChild nodeName]" == "loop" } {
                putLoopStart $fd $valuesChild
                foreach loopChild [$valuesChild childNodes] {
                    if { "[$loopChild nodeName]" == "value" } {
                        putValue $fd $loopChild "    "
                    } elseif { "[$loopChild nodeType]" == "COMMENT_NODE" } {
                        puts $fd "    //[$loopChild nodeValue]"
                        puts $fd ""
                    }
                }
                putLoopEnd $fd
            } elseif { "[$valuesChild nodeType]" == "COMMENT_NODE" } {
                puts $fd ""
                puts $fd "//[$valuesChild nodeValue]"
            }
        }
    }
}

close $fd
