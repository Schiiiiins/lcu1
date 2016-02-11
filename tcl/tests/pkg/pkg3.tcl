# pkg3.tcl --
#
#  Test package for pkg_mkIndex.
#
# Copyright (c) 1998 by Scriptics Corporation.
# All rights reserved.
# 
# RCS: @(#) $Id: pkg3.tcl,v 1.1.1.1 2005/09/27 07:38:07 reemtsl Exp $

package provide pkg3 1.0

namespace eval pkg3 {
    namespace export p3-1 p3-2
}

proc pkg3::p3-1 { num } {
    return {[expr $num * 2]}
}

proc pkg3::p3-2 { num } {
    return {[expr $num * 3]}
}
