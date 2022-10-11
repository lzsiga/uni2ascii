#!/bin/sh
#-*- mode: Tcl;time-stamp-start:"TimeStamp[ 	]+\\\\?[\"<]+";-*-
# the next line restarts using wish \
exec wish $0 -- $@
set TimeStamp "2010-08-29 21:04:40 poser"
#
# Copyright (C) 2005-2008 William J. Poser (billposer@alum.mit.edu)
# This program is free software; you can redistribute it and/or modify
# it under the terms of version 3 of the GNU General Public License as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# A copy of the GNU General Public License is contained in the
# procedure "License" in this file.
# If it is not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# or go to the web page:  http://www.gnu.org/licenses/gpl.txt.

set Version "4.15"
encoding system utf-8
set DebugP 0;				# General debugging?
package require tablelist
package require msgcat
proc _ {s} {return [::msgcat::mc $s]};	# Define shorthand for gettext
set NonBinPath [file join /usr share uni2ascii];
fconfigure stdout -encoding utf-8

#set ColorSpecs(AsciiOptionEven,Background)	"\#E9C8AC"
set ColorSpecs(AsciiOptionEven,Background)	"\#d7c7ff"
set ColorSpecs(AsciiOptionOdd,Background)	"\#ffbb9f"
set ColorSpecs(BalloonHelp,Background)		white
set ColorSpecs(BalloonHelp,Foreground)		blue
set ColorSpecs(Button,Background)		"\#8BD664"
set ColorSpecs(Button,Foreground)		black
set ColorSpecs(Button,ActiveBackground)		LightBlue
set ColorSpecs(Button,ActiveForeground)		red
set ColorSpecs(CheckButton,Background)		coral
set ColorSpecs(CheckButton,ActiveForeground)	red
set ColorSpecs(CheckButton,ActiveBackground)	LightBlue
set ColorSpecs(CheckButton,Select)	red
#set ColorSpecs(Default,Background)		"\#DEDEFF"
set ColorSpecs(Default,Background)		"\#c8dcfe"
set ColorSpecs(DefaultActive,Background)	"\#d4e4fe";
set ColorSpecs(Entry,Background)	"navajo white"
set ColorSpecs(Entry,DisabledBackground)		blue
set ColorSpecs(LabelText,Foreground)		"\#d4e4fe";
set ColorSpecs(ListHeader,Background)	"\#CBD7FF"
set ColorSpecs(ListHeader,Foreground)	black
set ColorSpecs(ListHeader,ActiveBackground)	"\#8f89fe";
set ColorSpecs(ListHeader,ActiveForeground)	black
set ColorSpecs(MainFrame,Background)		grey15
set ColorSpecs(Menu,ActiveBackground)	        salmon;
set ColorSpecs(Menu,ActiveForeground)	        maroon
set ColorSpecs(Menu,Background)			azure3
set ColorSpecs(Menu,Foreground)			black
set ColorSpecs(Menu,Select)			coral
set ColorSpecs(Menubar,ActiveBackground)	RoyalBlue2
set ColorSpecs(Menubar,ActiveForeground)	red
set ColorSpecs(Menubar,Background)	 	"\#c36176";
set ColorSpecs(Menubar,Foreground)		"\#fee4a9";
set ColorSpecs(Messages,Background) 		NavajoWhite
set ColorSpecs(Messages,Foreground) 		"\#000000";
set ColorSpecs(RadioButton,Background) "\#A0A0FF"
set ColorSpecs(RadioButton,ActiveForeground)	red
set ColorSpecs(RadioButton,ActiveBackground)	LightBlue
set ColorSpecs(RadioButton,SelectedBackground) red
set ColorSpecs(RadioButton,Select)	red
set ColorSpecs(UserTextEntry,Background)	"\#ffaf80"
set ColorSpecs(UserTextEntry,Foreground)	black
set ColorSpecs(TableList,SelectBackground)	"\#FFCFBF"
set ColorSpecs(TableList,SelectForeground)	black

option add *Background $ColorSpecs(Default,Background)
option add *Balloonhelp*background $ColorSpecs(BalloonHelp,Background) 80
option add *Balloonhelp*foreground $ColorSpecs(BalloonHelp,Foreground) 80
option add *Button.Background  $ColorSpecs(Button,Background)
option add *Button.Foreground  $ColorSpecs(Button,Foreground)
option add *Menu.Background $ColorSpecs(Menu,Background)
option add *Menu.Foreground $ColorSpecs(Menu,Foreground)
option add *Menu.activeBackground $ColorSpecs(Menu,ActiveBackground)
option add *Menu.activeForeground $ColorSpecs(Menu,ActiveForeground)
option add *Menu.SelectBackground purple
option add *Radiobutton.activeBackground  $ColorSpecs(RadioButton,ActiveBackground)
option add *Radiobutton.activeForeground  $ColorSpecs(RadioButton,ActiveForeground)
option add *Radiobutton.selectColor  $ColorSpecs(RadioButton,Select) 80

option add *Checkbutton*background $ColorSpecs(CheckButton,Background)
option add *Checkbutton.activeBackground  $ColorSpecs(CheckButton,ActiveBackground)
option add *Checkbutton.activeForeground  $ColorSpecs(CheckButton,ActiveForeground)
option add *Checkbutton.selectColor  $ColorSpecs(CheckButton,Select)

option add *Text.Font MainFont 100
option add *Menu.Font MenuFont 100

option add *Balloonhelpinfo.wrapLength 3i  widgetDefault
option add *Balloonhelp.info.justify left widgetDefault

font create BalloonHelpFont -family lucida -size 12
font create MenuFont -family courier -size 12
font create MainFont -family courier -size 13

set MSG .mf.msg

#For debugging messages
proc dmsg {msg} {
    if {$::DebugP} {
	puts $msg;
	flush stdout;
    }
}

proc ShowMessage {msg} {
    if {[winfo exists $::MSG]} {
	$::MSG configure -state normal;
	$::MSG delete 1.0 end;
	$::MSG insert 1.0 $msg;
	$::MSG configure -state disabled;
	return
    }
    puts stderr $msg;
 }

 proc ClearMessage {} {
      $::MSG configure -state normal
      $::MSG delete 1.0 end;
      $::MSG configure -state disabled
}

#Portability
#Figure out what system we are running on
if {[string equal $tcl_platform(platform) windows]} {
    set SystemInfo(System) MSWindows;
    dmsg "Running under MS Windows";
} elseif {[string equal $tcl_platform(platform) unix]} {
    if {[string equal $tcl_platform(os) Darwin]} {
	set SystemInfo(System) MacOSX;
	dmsg "Running under Mac OS X";
    } else {
	set SystemInfo(System) Unix;
	dmsg "Running under Unix";
    }
}

if {[string match $SystemInfo(System) MSWindows]} {
    set NonBinPath [file dirname [info script]];
}

if {[info exists ::env(TMP)]} {
    set TempDir $::env(TMP);
} elseif {[info exists ::env(TEMP)]} {
    set TempDir $::env(TEMP);
} else {
    if { $SystemInfo(System)== "MSWindows"} {
	set TempDir "C:\tmp";
    } else {
	set TempDir "/tmp";
    }
}
set ResultFile [file join $TempDir "a2u.log"]

proc SetupEvents {sys} {
    switch $sys {
	Unix {
	    event add <<B3>> <ButtonPress-3>
	    event add <<B3Release>> <ButtonRelease-3>
	}
	MacOSX {
	    event add <<B3>> <Control-ButtonPress-1>
	    event add <<B3Release>> <Control-ButtonRelease-1>
	}
	MSWindows {
	    event add <<B3>> <ButtonPress-3>
	    event add <<B3Release>> <ButtonRelease-3>
	}
    }
}

proc DetermineGraphicsSystem {} {
    global SystemInfo

    if {[string match X11*  [winfo server .]]} {
	set SystemInfo(AquaP) 0
	set SystemInfo(WindowSystem) X11
    } else {
	if {[string match $SystemInfo(System) "MSWindows"]} {
	    set SystemInfo(AquaP) 0;
	    set SystemInfo(WindowSystem) MSWindows;
	}
	if {[string match $SystemInfo(System) "MacOSX"]} {
	    set SystemInfo(AquaP) 1
	    set SystemInfo(WindowSystem) Aqua
	}
    }
}

DetermineGraphicsSystem
SetupEvents $SystemInfo(System)

set SubstitutionList {}
set DeletionList {}

#Determines which program to use
# 1 = ascii2uni
# 0 = uni2ascii
set WhichWay 0
#Daughter program options the user may set
set HexUpperCaseP 1
set PreserveNewlinesP 1
set PreserveSpaceP 1
set ConvertAsciiP 0
set AddSpaceP 0
set InputPureP 0
set AcceptWithoutSemicolonP 1;	# Convert semi-colon-less entities and references
set WhichConversion 7
set ConvertStyleP 0
set ConvertEnclosedP 0
set StripDiacriticsP 0
set ConvertApproximateP 0
set ExpandToAsciiP 0

set BalloonHelpP 1

set DefaultBrowser dillo
set BrowserList [list firefox mozilla epiphany galeon konqueror dillo netscape]
set BrowserPIDS [list];

proc ShutDown {} {
    global BrowserPIDS;
    #Shutdown child browsers
    foreach pid $BrowserPIDS {
	catch {exec kill $pid};
    }
    exit 0;
}

proc ShowWebPage {url} {
    global BrowserPIDS;
    global SystemInfo
    if {[string equal $SystemInfo(System) MacOSX]} {
	lappend BrowserPIDS [exec osascript -e "\"open location $url\""]
	return 
    }
    set BrowserFound 0;
    foreach Browser $::BrowserList {
	if { [string length [auto_execok $Browser]]} {
	    set BrowserFound 1;
	    break ;
	} else {
	    ShowMessage [format \
	     [_ "The browser %s is not available on this machine or not in your path."]\
		 $Browser];
	}
    }
    if {$BrowserFound} {
	lappend BrowserPIDS [exec $Browser $url &]
    } else {
	ShowMessage [_ "No browser on the browser list was located."]
    }
}

proc new_dialog_create {class {win "auto"}} {
    if {$win == "auto"} {
        set count 0
        set win ".ndialog[incr count]"
        while {[winfo exists $win]} {
            set win ".ndialog[incr count]"
        }
    }
    toplevel $win -class $class;
    frame $win.info
    pack $win.info -expand yes -fill both -padx 4 -pady 4
    wm title $win $class
    wm group $win .

    after idle [format {
        update idletasks
        wm minsize %s [winfo reqwidth %s] [winfo reqheight %s]
    } $win $win $win]

    return $win
}

# The following code is taken from the Efftcl library by Mark Harrison and
# Michael McLennan, copyrighted by Mark Harrison and Lucent Technologies, available
# from http://www.awprofessional.com/content/images/0201634740/sourcecode/efftcl.zip.
# As the authors explicitly give permission to "steal the code for your own applications"
# the relevant portions are included here so as not to require the user to install
# to install the library. If you install the library, remove the following and
# uncomment the line "#package require Efftcl" by deleting the crosshatch.

#  Effective Tcl/Tk Programming
#    Mark Harrison, DSC Communications Corp.
#    Michael McLennan, Bell Labs Innovations for Lucent Technologies
#    Addison-Wesley Professional Computing Series
# ======================================================================
#  Copyright (c) 1996-1997  Lucent Technologies Inc. and Mark Harrison
# ======================================================================

proc dialog_info {win} {
    return "$win.info"
}

proc dialog_controls {win} {
    return "$win.controls"
}

proc dialog_wait {win varName} {
    dialog_safeguard $win

    set x [expr [winfo rootx .]+50]
    set y [expr [winfo rooty .]+50]
    wm geometry $win "+$x+$y"

    wm deiconify $win
    grab set $win

    vwait $varName

    grab release $win
    wm withdraw $win
}

bind modalDialog <ButtonPress> {
    wm deiconify %W
    raise %W
}
proc dialog_safeguard {win} {
    if {[lsearch [bindtags $win] modalDialog] < 0} {
        bindtags $win [linsert [bindtags $win] 0 modalDialog]
    }
}

proc CreateTextDisplay {title width height {bg "\#e6b483"} {fg "\#000080"} } {
    set top [new_dialog_create Textdisplay]
    wm title $top $title
    set info [dialog_info $top]
    scrollbar $info.sbar -command "$info.text yview" 
    pack $info.sbar -side right -fill y
    text $info.text -height $height -width $width -font MainFont -wrap word -yscrollcommand "$info.sbar set"  -background $bg -foreground $fg -exportselection 1;
    pack $info.text -side left -expand yes -fill both
    $info.text configure -state disabled
    bind $info.sbar <<B3>> "ScrollbarMoveBigIncrement $info.sbar 0.2 %x %y"
    return $top
}

proc PopupDown {n} {
    global HelpPopups;
    if {[info exists HelpPopups($n)]} {
	if {[winfo exists $HelpPopups($n)]} {
	    destroy $HelpPopups($n);
	    return 1;
	}
    }
    return 0;
}

proc AppendToTextDisplay {top mesg} {
    set info [dialog_info $top]
    $info.text configure -state normal
    $info.text insert end $mesg
    $info.text configure -state disabled
}

set linkNum 0;
proc AppendLinkToTextDisplay {top mesg LinkCode} {
    global linkNum
    set info [dialog_info $top]
    $info.text configure -state normal
    set tag "link[incr linkNum]"
    bind $info.text <ButtonPress> break
    $info.text insert end $mesg [list body $tag]
    $info.text tag configure $tag -foreground red -underline 1
    $info.text tag bind $tag <Enter> \
        "$info.text tag configure $tag -foreground blue"
    $info.text tag bind $tag <Leave> \
        "$info.text tag configure $tag -foreground red"
    $info.text tag bind $tag <ButtonPress> \
        "$LinkCode"
    $info.text configure -state disabled
}

proc ExplainEquivalences {} {
    global Version;
    if {[PopupDown ExplainEquivalences] ==1} {return}
    set po [CreateTextDisplay [_ "Equivalences"] 72 12]
    set ::HelpPopups(ExplainEquivalences) $po
    AppendToTextDisplay $po "U+00A0  no break space                              0x20  space\n"
    AppendToTextDisplay $po "U+00AB  left-pointing double angle quotation mark   0x22  double quote\n"
    AppendToTextDisplay $po "U+00AD  soft hyphen                                 0x2D  minus\n"
    AppendToTextDisplay $po "U+00BB  right-pointing double angle quotation mark  0x22  double quote\n"
    AppendToTextDisplay $po "U+1361  ethiopic word space                         0x20  space\n"
    AppendToTextDisplay $po "U+1680  ogham space                                 0x20  space\n"
    AppendToTextDisplay $po "U+2000  en quad                                     0x20  space\n"
    AppendToTextDisplay $po "U+2001  em quad                                     0x20  space\n"
    AppendToTextDisplay $po "U+2002  en space                                    0x20  space\n"
    AppendToTextDisplay $po "U+2003  em space                                    0x20  space\n"
    AppendToTextDisplay $po "U+2004  three-per-em space                          0x20  space\n"
    AppendToTextDisplay $po "U+2005  four-per-em space                           0x20  space\n"
    AppendToTextDisplay $po "U+2006  six-per-em space                            0x20  space\n"
    AppendToTextDisplay $po "U+2007  figure space                                0x20  space\n"
    AppendToTextDisplay $po "U+2008  punctuation space                           0x20  space\n"
    AppendToTextDisplay $po "U+2009  thin space                                  0x20  space\n"
    AppendToTextDisplay $po "U+200A  hair space                                  0x20  space\n"
    AppendToTextDisplay $po "U+200B  zero-width space                            0x20  space\n"
    AppendToTextDisplay $po "U+2010  hyphen                                      0x2D  minus\n"
    AppendToTextDisplay $po "U+2011  non-breaking hyphen                         0x2D  minus\n"
    AppendToTextDisplay $po "U+2012  figure dash                                 0x2D  minus\n"
    AppendToTextDisplay $po "U+2013  en dash                                     0x2D  minus\n"
    AppendToTextDisplay $po "U+2014  em dash                                     0x2D  minus\n"
    AppendToTextDisplay $po "U+2018  left single quotation mark                  0x60  left single quote\n"
    AppendToTextDisplay $po "U+2019  right single quotation mark                 0x27  right or neutral single quote\n"
    AppendToTextDisplay $po "U+201A  single low-9 quotation mark                 0x60  left single quote\n"
    AppendToTextDisplay $po "U+201B  single high-reversed-9 quotation mark       0x60  left single quote\n"
    AppendToTextDisplay $po "U+201C  left double quotation mark                  0x22  double quote\n"
    AppendToTextDisplay $po "U+201D  right double quotation mark                 0x22  double quote\n"
    AppendToTextDisplay $po "U+201E  double low-9 quotation mark                 0x22  double quote\n"
    AppendToTextDisplay $po "U+201F  double high-reversed-9 quotation mark       0x22  double quote\n"
    AppendToTextDisplay $po "U+2039  single left-pointing angle quotation mark   0x60  left single quote\n"
    AppendToTextDisplay $po "U+203A  single right-pointing angle quotation mark  0x27  right or neutral single quote\n"
    AppendToTextDisplay $po "U+204E  low asterisk                                0x2A  asterisk\n"
    AppendToTextDisplay $po "U+2212  minus sign                                  0x2D  minus\n"
    AppendToTextDisplay $po "U+2217  asterisk operator                           0x2A  asterisk\n"
    AppendToTextDisplay $po "U+222A  union symbol                                0x55  capital U\n"
    AppendToTextDisplay $po "U+2731  heavy asterisk                              0x2A  asterisk\n"
    AppendToTextDisplay $po "U+275D  heavy double turned comma quotation mark    0x22  double quote\n"
    AppendToTextDisplay $po "U+275E  heavy double comma quotation mark           0x22  double quote\n"
    AppendToTextDisplay $po "U+3000  ideographic space                           0x20  space\n"
    AppendToTextDisplay $po "U+FE60  small ampersand                             0x26  ampersand\n"
    AppendToTextDisplay $po "U+FE61  small asterisk                              0x2A  asterisk\n"
    AppendToTextDisplay $po "U+FE62  small plus sign                             0x2B  plus sign\n";
}

proc ExplainExpansions {} {
    global Version;
    if {[PopupDown ExplainExpansions] ==1} {return}
    set po [CreateTextDisplay [_ "Expansions"] 72 12]
    set ::HelpPopups(ExplainExpansions) $po
    AppendToTextDisplay $po "If this option is chosen, certain characters are expanded to sequences of approximately equivalent plain ASCII characters. The expansions are:\n"
    AppendToTextDisplay $po "\tU+00A2 \u00A2\tCENT SIGN         \t\u2192 cent\n"
    AppendToTextDisplay $po "\tU+00A3 \u00A3\tPOUND SIGN        \t\u2192 pound\n"
    AppendToTextDisplay $po "\tU+00A5 \u00A5\tYEN SIGN          \t\u2192 yen\n"
    AppendToTextDisplay $po "\tU+00A9 \u00A9\tCOPYRIGHT SYMBOL         \t\u2192 (c)\n"
    AppendToTextDisplay $po "\tU+00AE \u00AE\tREGISTERED SYMBOL        \t\u2192 (R)\n"
    AppendToTextDisplay $po "\tU+00BC \u00BC\tONE QUARTER              \t\u2192 1/4\n"
    AppendToTextDisplay $po "\tU+00BD \u00BD\tONE HALF                 \t\u2192 1/2\n"
    AppendToTextDisplay $po "\tU+00BE \u00BE\tTHREE QUARTERS           \t\u2192 3/4\n"
    AppendToTextDisplay $po "\tU+00C6 \u00C6\tCAPITAL LETTER ASH       \t\u2192 AE\n"
    AppendToTextDisplay $po "\tU+00DF \u00DF\tSMALL LETTER SHARP S     \t\u2192 ss\n"
    AppendToTextDisplay $po "\tU+00E6 \u00E6\tSMALL LETTER ASH         \t\u2192 ae\n"
    AppendToTextDisplay $po "\tU+0132 \u0132\tLIGATURE IJ              \t\u2192 IJ\n"
    AppendToTextDisplay $po "\tU+0133 \u0133\tLIGATURE ij              \t\u2192 ij\n"
    AppendToTextDisplay $po "\tU+0152 \u0152\tLIGATURE OE              \t\u2192 OE\n"
    AppendToTextDisplay $po "\tU+0153 \u0153\tLIGATURE oe              \t\u2192 oe\n"
    AppendToTextDisplay $po "\tU+01F1 \u01F1\tCAPITAL LETTER DZ        \t\u2192 DZ\n"
    AppendToTextDisplay $po "\tU+01F2 \u01F2\tCAPITAL LETTER Dz        \t\u2192 Dz\n"
    AppendToTextDisplay $po "\tU+01F3 \u01F3\tSMALLL LETTER DZ         \t\u2192 dz\n"
    AppendToTextDisplay $po "\tU+02A6 \u02A6\tSMALLL LETTER TS DIGRAPH \t\u2192 ts\n"
    AppendToTextDisplay $po "\tU+2026 \u2026\tHORIZONTAL ELLIPSIS      \t\u2192 ...\n"
    AppendToTextDisplay $po "\tU+20AC \u20AC\tEURO SIGN                \t\u2192 euro\n"
    AppendToTextDisplay $po "\tU+2190 \u2190\tLEFTWARDS ARROW          \t\u2192 <-\n"
    AppendToTextDisplay $po "\tU+2192 \u2192\tRIGHTWARDS ARROW         \t\u2192 ->\n"
    AppendToTextDisplay $po "\tU+21D0 \u21D0\tLEFTWARDS DOUBLE ARROW   \t\u2192 <=\n"
    AppendToTextDisplay $po "\tU+21D2 \u21D2\tRIGHTWARDS DOUBLE ARROW  \t\u2192 =>\n"
    AppendToTextDisplay $po "\tU+22EF \u22EF\tMIDLINE HORIZONTAL ELLIPSIS\t\u2192 ...\n"
}

proc ExplainSingleApproximations {} {
    global Version;
    if {[PopupDown ExplainSingleApproximations] ==1} {return}
    set po [CreateTextDisplay [_ "Single Character Approximations"] 72 12]
    set ::HelpPopups(ExplainSingleApproximations) $po
    AppendToTextDisplay $po "If this option is chosen, certain characters are converted to a single equivalent plain ASCII character. The conversions are:\n"
    AppendToTextDisplay $po "\tU+00A2 \u00A2\tCENT SIGN         \t\u2192 C\n"
    AppendToTextDisplay $po "\tU+00A3 \u00A3\tPOUND SIGN        \t\u2192 \#\n"
    AppendToTextDisplay $po "\tU+00A5 \u00A5\tYEN SIGN          \t\u2192 Y\n"
    AppendToTextDisplay $po "\tU+00A9 \u00A9\tCOPYRIGHT SYMBOL         \t\u2192 C\n"
    AppendToTextDisplay $po "\tU+00AE \u00AE\tREGISTERED SYMBOL        \t\u2192 R\n"
    AppendToTextDisplay $po "\tU+00BC \u00BC\tONE QUARTER              \t\u2192 -\n"
    AppendToTextDisplay $po "\tU+00BD \u00BD\tONE HALF                 \t\u2192 -\n"
    AppendToTextDisplay $po "\tU+00BE \u00BE\tTHREE QUARTERS           \t\u2192 -\n"
    AppendToTextDisplay $po "\tU+00C6 \u00C6\tCAPITAL LETTER ASH       \t\u2192 A\n"
    AppendToTextDisplay $po "\tU+00DF \u00DF\tSMALL LETTER SHARP S     \t\u2192 s\n"
    AppendToTextDisplay $po "\tU+00E6 \u00E6\tSMALL LETTER ASH         \t\u2192 a\n"
    AppendToTextDisplay $po "\tU+0132 \u0132\tLIGATURE IJ              \t\u2192 I\n"
    AppendToTextDisplay $po "\tU+0133 \u0133\tLIGATURE ij              \t\u2192 i\n"
    AppendToTextDisplay $po "\tU+0152 \u0152\tLIGATURE OE              \t\u2192 O\n"
    AppendToTextDisplay $po "\tU+0153 \u0153\tLIGATURE oe              \t\u2192 o\n"
    AppendToTextDisplay $po "\tU+01F1 \u01F1\tCAPITAL LETTER DZ        \t\u2192 D\n"
    AppendToTextDisplay $po "\tU+01F2 \u01F2\tCAPITAL LETTER Dz        \t\u2192 D\n"
    AppendToTextDisplay $po "\tU+01F3 \u01F3\tSMALLL LETTER DZ         \t\u2192 d\n"
    AppendToTextDisplay $po "\tU+02A6 \u02A6\tSMALLL LETTER TS DIGRAPH \t\u2192 t\n"
    AppendToTextDisplay $po "\tU+2026 \u2026\tHORIZONTAL ELLIPSIS      \t\u2192 .\n"
    AppendToTextDisplay $po "\tU+20AC \u20AC\tEURO SIGN                \t\u2192 E\n"
    AppendToTextDisplay $po "\tU+2190 \u2190\tLEFTWARDS ARROW          \t\u2192 <\n"
    AppendToTextDisplay $po "\tU+2192 \u2192\tRIGHTWARDS ARROW         \t\u2192 >\n"
    AppendToTextDisplay $po "\tU+21D0 \u21D0\tLEFTWARDS DOUBLE ARROW   \t\u2192 <\n"
    AppendToTextDisplay $po "\tU+21D2 \u21D2\tRIGHTWARDS DOUBLE ARROW  \t\u2192 >\n"
    AppendToTextDisplay $po "\tU+22EF \u22EF\tMIDLINE HORIZONTAL ELLIPSIS\t\u2192 .\n"
}


proc About {} {
    global Version;
    if {[PopupDown About] ==1} {return}
    set AboutPopup [CreateTextDisplay [_ "About This Program"] 72 12]
    set ::HelpPopups(About) $AboutPopup;
    AppendToTextDisplay $AboutPopup [format [_ "This is U2A version %s. "] $Version];
    AppendToTextDisplay $AboutPopup [_ "It converts between Unicode and various textual representations of Unicode."];
    AppendToTextDisplay $AboutPopup [_ "You can obtain the latest version of U2A from: "];
    AppendLinkToTextDisplay $AboutPopup "http://billposer.org/Software/uni2ascii.html." {ShowWebPage http://billposer.org/Software/uni2ascii.html};
    AppendToTextDisplay $AboutPopup "\n\n";
    AppendToTextDisplay $AboutPopup "Copyright (C) 2005-2008 William J. Poser (billposer@alum.mit.edu). ";
    AppendToTextDisplay $AboutPopup [_ "This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version."];
    AppendToTextDisplay $AboutPopup "\n\n";
    AppendToTextDisplay $AboutPopup [_ "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"];
    AppendToTextDisplay $AboutPopup [_ "See the GNU General Public License for more details."];
    AppendToTextDisplay $AboutPopup "\n\n";
    AppendToTextDisplay $AboutPopup [_ "You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA."];
}


set BugReportPopup "";
proc BugReports {} {
    global Version;
    global tcl_platform;
    global BugReportPopup

    if {[PopupDown BugReports] ==1} {return}
    set BugReportPopup [CreateTextDisplay [_ "Bug Reports"] 50 21];
    set ::HelpPopups(BugReports) $BugReportPopup;
    AppendToTextDisplay $BugReportPopup [_ "Report bugs to: billposer@alum.mit.edu.\n"];
    AppendToTextDisplay $BugReportPopup [_ "Please include the following information:\n\n"];
    AppendToTextDisplay $BugReportPopup [_ "\tWhat version of U2A are you using?\n"];
    AppendToTextDisplay $BugReportPopup [format [_ "\t\t(This is version %s.)\n\n"] $Version];
    AppendToTextDisplay $BugReportPopup [_ "\tWhat operating system are you running?.\n"];
    set OS $tcl_platform(os);
    if {$OS == "Linux"} {set OS "GNU/Linux"};
    AppendToTextDisplay $BugReportPopup [format [_ "\t\t(This is %s  %s.)\n\n"] $OS  $tcl_platform(osVersion)];
    AppendToTextDisplay $BugReportPopup [_ "\tWhat window system are you using?.\n"];
    AppendToTextDisplay $BugReportPopup [format [_ "\t\t(This is %s.)\n\n"] $::SystemInfo(WindowSystem)]
    AppendToTextDisplay $BugReportPopup [_ "\tWhat version of tcl/tk are you using?.\n"];
    AppendToTextDisplay $BugReportPopup [format [_ "\t\t(This is version %s.)\n\n"] [info patchlevel]];
    AppendToTextDisplay $BugReportPopup [_ "Bug reports may be sent in any language that I can read without too much trouble or am trying to learn or improve. These include:\n\n"];
    AppendLinkToTextDisplay $BugReportPopup [_ "\tCatalan"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=cat};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tDakelh (Carrier)"] {ShowWebPage http://ydli.org}
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tDutch"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=nld};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tEnglish"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=eng};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tEsperanto"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=epo};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tFrench"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=fra};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tGerman"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=deu};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tItalian"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=ita};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tJapanese"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=jpn};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tKazakh"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=kaz};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tKorean"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=kor};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tLatin"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=lat};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tPortuguese"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=por};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tSpanish"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=spa};
    AppendToTextDisplay $BugReportPopup "\n";
    AppendLinkToTextDisplay $BugReportPopup [_ "\tTurkish"] {ShowWebPage http://www.ethnologue.com/show_language.asp?code=tur};
    AppendToTextDisplay $BugReportPopup "\n\n";
    AppendToTextDisplay $BugReportPopup [_ "Please note that in many cases although I can understand the language my ability to respond in it may be limited.\n"];
    AppendToTextDisplay $BugReportPopup "\n";
}

proc ShowGPL {} {
    if {[PopupDown ShowGPL] ==1} {return}
    set po [CreateTextDisplay [_ "License"] 70 24]
    set ::PopupList(ShowGPL) $po;
    AppendToTextDisplay $po [format "%s%s" [format "%s\n\t%s\n" [_ "For this license in your language see:"] [_ "http://www.gnu.org/copyleft/gpl.html"]]  "\
\

		GNU GENERAL PUBLIC LICENSE\

   Version 3, 29 June 2007\


 Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
\

Preamble
\

  The GNU General Public License is a free, copyleft license for
software and other kinds of works.
\

  The licenses for most software and other practical works are designed
to take away your freedom to share and change the works.  By contrast,
the GNU General Public License is intended to guarantee your freedom to
share and change all versions of a program--to make sure it remains free
software for all its users.  We, the Free Software Foundation, use the
GNU General Public License for most of our software; it applies also to
any other work released this way by its authors.  You can apply it to
your programs, too.
\

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
							them if you wish), that you receive source code or can get it if you
want it, that you can change the software or use pieces of it in new
free programs, and that you know you can do these things.
\

To protect your rights, we need to prevent others from denying you
these rights or asking you to surrender the rights.  Therefore, you have
certain responsibilities if you distribute copies of the software, or if
you modify it: responsibilities to respect the freedom of others.
\

For example, if you distribute copies of such a program, whether
gratis or for a fee, you must pass on to the recipients the same
freedoms that you received.  You must make sure that they, too, receive
or can get the source code.  And you must show them these terms so they
know their rights.
\

Developers that use the GNU GPL protect your rights with two steps:
(1) assert copyright on the software, and (2) offer you this License
giving you legal permission to copy, distribute and/or modify it.
\

For the developers' and authors' protection, the GPL clearly explains
that there is no warranty for this free software.  For both users' and
authors' sake, the GPL requires that modified versions be marked as
changed, so that their problems will not be attributed erroneously to
authors of previous versions.
\

Some devices are designed to deny users access to install or run
modified versions of the software inside them, although the manufacturer
can do so.  This is fundamentally incompatible with the aim of
protecting users' freedom to change the software.  The systematic
pattern of such abuse occurs in the area of products for individuals to
use, which is precisely where it is most unacceptable.  Therefore, we
have designed this version of the GPL to prohibit the practice for those
products.  If such problems arise substantially in other domains, we
stand ready to extend this provision to those domains in future versions
of the GPL, as needed to protect the freedom of users.
\

Finally, every program is threatened constantly by software patents.
States should not allow patents to restrict development and use of
software on general-purpose computers, but in those that do, we wish to
avoid the special danger that patents applied to a free program could
make it effectively proprietary.  To prevent this, the GPL assures that
patents cannot be used to render the program non-free.
\

The precise terms and conditions for copying, distribution and
modification follow.
\

TERMS AND CONDITIONS
\

0. Definitions.
\

\"This License\" refers to version 3 of the GNU General Public License.
\

\"Copyright\" also means copyright-like laws that apply to other kinds of
works, such as semiconductor masks.
\

\"The Program\" refers to any copyrightable work licensed under this
License.  Each licensee is addressed as \"you\".  \"Licensees\" and
\"recipients\" may be individuals or organizations.
\

To \"modify\" a work means to copy from or adapt all or part of the work
in a fashion requiring copyright permission, other than the making of an
exact copy.  The resulting work is called a \"modified version\" of the
earlier work or a work \"based on\" the earlier work.
\

A \"covered work\" means either the unmodified Program or a work based
on the Program.
\

To \"propagate\" a work means to do anything with it that, without
permission, would make you directly or secondarily liable for
infringement under applicable copyright law, except executing it on a
computer or modifying a private copy.  Propagation includes copying,
distribution (with or without modification), making available to the
public, and in some countries other activities as well.
\

To \"convey\" a work means any kind of propagation that enables other
parties to make or receive copies.  Mere interaction with a user through
a computer network, with no transfer of a copy, is not conveying.
\

An interactive user interface displays \"Appropriate Legal Notices\"
to the extent that it includes a convenient and prominently visible
feature that (1) displays an appropriate copyright notice, and (2)
tells the user that there is no warranty for the work (except to the
						       extent that warranties are provided), that licensees may convey the
work under this License, and how to view a copy of this License.  If
the interface presents a list of user commands or options, such as a
menu, a prominent item in the list meets this criterion.
\

1. Source Code.
\

The \"source code\" for a work means the preferred form of the work
for making modifications to it.  \"Object code\" means any non-source
form of a work.
\

A \"Standard Interface\" means an interface that either is an official
standard defined by a recognized standards body, or, in the case of
interfaces specified for a particular programming language, one that
is widely used among developers working in that language.
\

The \"System Libraries\" of an executable work include anything, other
than the work as a whole, that (a) is included in the normal form of
packaging a Major Component, but which is not part of that Major
Component, and (b) serves only to enable use of the work with that
Major Component, or to implement a Standard Interface for which an
implementation is available to the public in source code form.  A
\"Major Component\", in this context, means a major essential component
(kernel, window system, and so on) of the specific operating system
(if any) on which the executable work runs, or a compiler used to
produce the work, or an object code interpreter used to run it.
\

The \"Corresponding Source\" for a work in object code form means all
the source code needed to generate, install, and (for an executable
						  work) run the object code and to modify the work, including scripts to
control those activities.  However, it does not include the work's
System Libraries, or general-purpose tools or generally available free
programs which are used unmodified in performing those activities but
which are not part of the work.  For example, Corresponding Source
includes interface definition files associated with source files for
the work, and the source code for shared libraries and dynamically
linked subprograms that the work is specifically designed to require,
such as by intimate data communication or control flow between those
subprograms and other parts of the work.
\

The Corresponding Source need not include anything that users
can regenerate automatically from other parts of the Corresponding
Source.
\

The Corresponding Source for a work in source code form is that
same work.
\

2. Basic Permissions.
\

All rights granted under this License are granted for the term of
copyright on the Program, and are irrevocable provided the stated
conditions are met.  This License explicitly affirms your unlimited
permission to run the unmodified Program.  The output from running a
covered work is covered by this License only if the output, given its
content, constitutes a covered work.  This License acknowledges your
rights of fair use or other equivalent, as provided by copyright law.
\

You may make, run and propagate covered works that you do not
convey, without conditions so long as your license otherwise remains
in force.  You may convey covered works to others for the sole purpose
of having them make modifications exclusively for you, or provide you
with facilities for running those works, provided that you comply with
the terms of this License in conveying all material for which you do
not control copyright.  Those thus making or running the covered works
for you must do so exclusively on your behalf, under your direction
and control, on terms that prohibit them from making any copies of
your copyrighted material outside their relationship with you.
\

Conveying under any other circumstances is permitted solely under
the conditions stated below.  Sublicensing is not allowed; section 10
makes it unnecessary.
\

3. Protecting Users' Legal Rights From Anti-Circumvention Law.
\

No covered work shall be deemed part of an effective technological
measure under any applicable law fulfilling obligations under article
11 of the WIPO copyright treaty adopted on 20 December 1996, or
similar laws prohibiting or restricting circumvention of such
measures.
\

When you convey a covered work, you waive any legal power to forbid
circumvention of technological measures to the extent such circumvention
is effected by exercising rights under this License with respect to
the covered work, and you disclaim any intention to limit operation or
modification of the work as a means of enforcing, against the work's
users, your or third parties' legal rights to forbid circumvention of
technological measures.
\

4. Conveying Verbatim Copies.
\

You may convey verbatim copies of the Program's source code as you
receive it, in any medium, provided that you conspicuously and
appropriately publish on each copy an appropriate copyright notice;
keep intact all notices stating that this License and any
non-permissive terms added in accord with section 7 apply to the code;
keep intact all notices of the absence of any warranty; and give all
recipients a copy of this License along with the Program.
\

You may charge any price or no price for each copy that you convey,
and you may offer support or warranty protection for a fee.
\

5. Conveying Modified Source Versions.
\

You may convey a work based on the Program, or the modifications to
produce it from the Program, in the form of source code under the
terms of section 4, provided that you also meet all of these conditions:
\

a) The work must carry prominent notices stating that you modified
it, and giving a relevant date.
\

b) The work must carry prominent notices stating that it is
released under this License and any conditions added under section
7.  This requirement modifies the requirement in section 4 to
\"keep intact all notices\".
\

c) You must license the entire work, as a whole, under this
License to anyone who comes into possession of a copy.  This
License will therefore apply, along with any applicable section 7
additional terms, to the whole of the work, and all its parts,
regardless of how they are packaged.  This License gives no
permission to license the work in any other way, but it does not
invalidate such permission if you have separately received it.
\

d) If the work has interactive user interfaces, each must display
Appropriate Legal Notices; however, if the Program has interactive
interfaces that do not display Appropriate Legal Notices, your
work need not make them do so.
\

A compilation of a covered work with other separate and independent
works, which are not by their nature extensions of the covered work,
and which are not combined with it such as to form a larger program,
in or on a volume of a storage or distribution medium, is called an
\"aggregate\" if the compilation and its resulting copyright are not
used to limit the access or legal rights of the compilation's users
beyond what the individual works permit.  Inclusion of a covered work
in an aggregate does not cause this License to apply to the other
parts of the aggregate.
\

6. Conveying Non-Source Forms.
\

You may convey a covered work in object code form under the terms
of sections 4 and 5, provided that you also convey the
machine-readable Corresponding Source under the terms of this License,
in one of these ways:
\

a) Convey the object code in, or embodied in, a physical product
(including a physical distribution medium), accompanied by the
Corresponding Source fixed on a durable physical medium
customarily used for software interchange.
\

b) Convey the object code in, or embodied in, a physical product
(including a physical distribution medium), accompanied by a
written offer, valid for at least three years and valid for as
long as you offer spare parts or customer support for that product
model, to give anyone who possesses the object code either (1) a
copy of the Corresponding Source for all the software in the
product that is covered by this License, on a durable physical
medium customarily used for software interchange, for a price no
more than your reasonable cost of physically performing this
conveying of source, or (2) access to copy the
Corresponding Source from a network server at no charge.
\

c) Convey individual copies of the object code with a copy of the
written offer to provide the Corresponding Source.  This
alternative is allowed only occasionally and noncommercially, and
only if you received the object code with such an offer, in accord
with subsection 6b.
\

d) Convey the object code by offering access from a designated
place (gratis or for a charge), and offer equivalent access to the
Corresponding Source in the same way through the same place at no
further charge.  You need not require recipients to copy the
Corresponding Source along with the object code.  If the place to
copy the object code is a network server, the Corresponding Source
may be on a different server (operated by you or a third party)
that supports equivalent copying facilities, provided you maintain
clear directions next to the object code saying where to find the
Corresponding Source.  Regardless of what server hosts the
Corresponding Source, you remain obligated to ensure that it is
available for as long as needed to satisfy these requirements.
\

e) Convey the object code using peer-to-peer transmission, provided
you inform other peers where the object code and Corresponding
Source of the work are being offered to the general public at no
charge under subsection 6d.
\

A separable portion of the object code, whose source code is excluded
from the Corresponding Source as a System Library, need not be
included in conveying the object code work.
\

A \"User Product\" is either (1) a \"consumer product\", which means any
tangible personal property which is normally used for personal, family,
or household purposes, or (2) anything designed or sold for incorporation
into a dwelling.  In determining whether a product is a consumer product,
doubtful cases shall be resolved in favor of coverage.  For a particular
product received by a particular user, \"normally used\" refers to a
typical or common use of that class of product, regardless of the status
of the particular user or of the way in which the particular user
actually uses, or expects or is expected to use, the product.  A product
is a consumer product regardless of whether the product has substantial
commercial, industrial or non-consumer uses, unless such uses represent
the only significant mode of use of the product.
\

\"Installation Information\" for a User Product means any methods,
procedures, authorization keys, or other information required to install
and execute modified versions of a covered work in that User Product from
a modified version of its Corresponding Source.  The information must
suffice to ensure that the continued functioning of the modified object
code is in no case prevented or interfered with solely because
modification has been made.
\

If you convey an object code work under this section in, or with, or
specifically for use in, a User Product, and the conveying occurs as
part of a transaction in which the right of possession and use of the
User Product is transferred to the recipient in perpetuity or for a
fixed term (regardless of how the transaction is characterized), the
Corresponding Source conveyed under this section must be accompanied
by the Installation Information.  But this requirement does not apply
if neither you nor any third party retains the ability to install
modified object code on the User Product (for example, the work has
					  been installed in ROM).
\

The requirement to provide Installation Information does not include a
requirement to continue to provide support service, warranty, or updates
for a work that has been modified or installed by the recipient, or for
the User Product in which it has been modified or installed.  Access to a
network may be denied when the modification itself materially and
adversely affects the operation of the network or violates the rules and
protocols for communication across the network.
\

Corresponding Source conveyed, and Installation Information provided,
in accord with this section must be in a format that is publicly
documented (and with an implementation available to the public in
	    source code form), and must require no special password or key for
unpacking, reading or copying.
\

7. Additional Terms.
\

\"Additional permissions\" are terms that supplement the terms of this
License by making exceptions from one or more of its conditions.
Additional permissions that are applicable to the entire Program shall
be treated as though they were included in this License, to the extent
that they are valid under applicable law.  If additional permissions
apply only to part of the Program, that part may be used separately
under those permissions, but the entire Program remains governed by
this License without regard to the additional permissions.
\

When you convey a copy of a covered work, you may at your option
remove any additional permissions from that copy, or from any part of
it.  (Additional permissions may be written to require their own
      removal in certain cases when you modify the work.)  You may place
additional permissions on material, added by you to a covered work,
for which you have or can give appropriate copyright permission.
\

Notwithstanding any other provision of this License, for material you
add to a covered work, you may (if authorized by the copyright holders of
				that material) supplement the terms of this License with terms:
\

a) Disclaiming warranty or limiting liability differently from the
terms of sections 15 and 16 of this License; or
\

b) Requiring preservation of specified reasonable legal notices or
author attributions in that material or in the Appropriate Legal
Notices displayed by works containing it; or
\

c) Prohibiting misrepresentation of the origin of that material, or
requiring that modified versions of such material be marked in
reasonable ways as different from the original version; or
\

d) Limiting the use for publicity purposes of names of licensors or
authors of the material; or
\

e) Declining to grant rights under trademark law for use of some
trade names, trademarks, or service marks; or
\

f) Requiring indemnification of licensors and authors of that
material by anyone who conveys the material (or modified versions of
					     it) with contractual assumptions of liability to the recipient, for
any liability that these contractual assumptions directly impose on
those licensors and authors.
\

All other non-permissive additional terms are considered \"further
restrictions\" within the meaning of section 10.  If the Program as you
received it, or any part of it, contains a notice stating that it is
governed by this License along with a term that is a further
restriction, you may remove that term.  If a license document contains
a further restriction but permits relicensing or conveying under this
License, you may add to a covered work material governed by the terms
of that license document, provided that the further restriction does
not survive such relicensing or conveying.
\

If you add terms to a covered work in accord with this section, you
must place, in the relevant source files, a statement of the
additional terms that apply to those files, or a notice indicating
where to find the applicable terms.
\

Additional terms, permissive or non-permissive, may be stated in the
form of a separately written license, or stated as exceptions;
the above requirements apply either way.
\

8. Termination.
\

You may not propagate or modify a covered work except as expressly
provided under this License.  Any attempt otherwise to propagate or
modify it is void, and will automatically terminate your rights under
this License (including any patent licenses granted under the third
	      paragraph of section 11).
\

However, if you cease all violation of this License, then your
license from a particular copyright holder is reinstated (a)
provisionally, unless and until the copyright holder explicitly and
finally terminates your license, and (b) permanently, if the copyright
holder fails to notify you of the violation by some reasonable means
prior to 60 days after the cessation.
\

Moreover, your license from a particular copyright holder is
reinstated permanently if the copyright holder notifies you of the
violation by some reasonable means, this is the first time you have
received notice of violation of this License (for any work) from that
copyright holder, and you cure the violation prior to 30 days after
your receipt of the notice.
\

Termination of your rights under this section does not terminate the
licenses of parties who have received copies or rights from you under
this License.  If your rights have been terminated and not permanently
reinstated, you do not qualify to receive new licenses for the same
material under section 10.
\

9. Acceptance Not Required for Having Copies.
\

You are not required to accept this License in order to receive or
run a copy of the Program.  Ancillary propagation of a covered work
occurring solely as a consequence of using peer-to-peer transmission
to receive a copy likewise does not require acceptance.  However,
nothing other than this License grants you permission to propagate or
modify any covered work.  These actions infringe copyright if you do
not accept this License.  Therefore, by modifying or propagating a
covered work, you indicate your acceptance of this License to do so.
\

10. Automatic Licensing of Downstream Recipients.

Each time you convey a covered work, the recipient automatically
receives a license from the original licensors, to run, modify and
propagate that work, subject to this License.  You are not responsible
for enforcing compliance by third parties with this License.
\

An \"entity transaction\" is a transaction transferring control of an
organization, or substantially all assets of one, or subdividing an
organization, or merging organizations.  If propagation of a covered
work results from an entity transaction, each party to that
transaction who receives a copy of the work also receives whatever
licenses to the work the party's predecessor in interest had or could
give under the previous paragraph, plus a right to possession of the
Corresponding Source of the work from the predecessor in interest, if
the predecessor has it or can get it with reasonable efforts.
\

You may not impose any further restrictions on the exercise of the
rights granted or affirmed under this License.  For example, you may
not impose a license fee, royalty, or other charge for exercise of
rights granted under this License, and you may not initiate litigation
(including a cross-claim or counterclaim in a lawsuit) alleging that
any patent claim is infringed by making, using, selling, offering for
sale, or importing the Program or any portion of it.
\

11. Patents.
\

A \"contributor\" is a copyright holder who authorizes use under this
License of the Program or a work on which the Program is based.  The
work thus licensed is called the contributor's \"contributor version\".
\

A contributor's \"essential patent claims\" are all patent claims
owned or controlled by the contributor, whether already acquired or
hereafter acquired, that would be infringed by some manner, permitted
by this License, of making, using, or selling its contributor version,
but do not include claims that would be infringed only as a
consequence of further modification of the contributor version.  For
purposes of this definition, \"control\" includes the right to grant
patent sublicenses in a manner consistent with the requirements of
this License.
\

Each contributor grants you a non-exclusive, worldwide, royalty-free
patent license under the contributor's essential patent claims, to
make, use, sell, offer for sale, import and otherwise run, modify and
propagate the contents of its contributor version.
\

In the following three paragraphs, a \"patent license\" is any express
agreement or commitment, however denominated, not to enforce a patent
(such as an express permission to practice a patent or covenant not to
 sue for patent infringement).  To \"grant\" such a patent license to a
party means to make such an agreement or commitment not to enforce a
patent against the party.
\

If you convey a covered work, knowingly relying on a patent license,
and the Corresponding Source of the work is not available for anyone
to copy, free of charge and under the terms of this License, through a
publicly available network server or other readily accessible means,
then you must either (1) cause the Corresponding Source to be so
available, or (2) arrange to deprive yourself of the benefit of the
patent license for this particular work, or (3) arrange, in a manner
consistent with the requirements of this License, to extend the patent
license to downstream recipients.  \"Knowingly relying\" means you have
actual knowledge that, but for the patent license, your conveying the
covered work in a country, or your recipient's use of the covered work
in a country, would infringe one or more identifiable patents in that
country that you have reason to believe are valid.
\

If, pursuant to or in connection with a single transaction or
arrangement, you convey, or propagate by procuring conveyance of, a
covered work, and grant a patent license to some of the parties
receiving the covered work authorizing them to use, propagate, modify
or convey a specific copy of the covered work, then the patent license
you grant is automatically extended to all recipients of the covered
work and works based on it.
\

A patent license is \"discriminatory\" if it does not include within
the scope of its coverage, prohibits the exercise of, or is
conditioned on the non-exercise of one or more of the rights that are
specifically granted under this License.  You may not convey a covered
work if you are a party to an arrangement with a third party that is
in the business of distributing software, under which you make payment
to the third party based on the extent of your activity of conveying
the work, and under which the third party grants, to any of the
parties who would receive the covered work from you, a discriminatory
patent license (a) in connection with copies of the covered work
conveyed by you (or copies made from those copies), or (b) primarily
for and in connection with specific products or compilations that
contain the covered work, unless you entered into that arrangement,
or that patent license was granted, prior to 28 March 2007.
\

Nothing in this License shall be construed as excluding or limiting
any implied license or other defenses to infringement that may
otherwise be available to you under applicable patent law.
\

12. No Surrender of Others' Freedom.
\

If conditions are imposed on you (whether by court order, agreement or
				  otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot convey a
covered work so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you may
not convey it at all.  For example, if you agree to terms that obligate you
to collect a royalty for further conveying from those to whom you convey
the Program, the only way you could satisfy both those terms and this
License would be to refrain entirely from conveying the Program.
\

13. Use with the GNU Affero General Public License.
\

Notwithstanding any other provision of this License, you have
permission to link or combine any covered work with a work licensed
under version 3 of the GNU Affero General Public License into a single
combined work, and to convey the resulting work.  The terms of this
License will continue to apply to the part which is the covered work,
but the special requirements of the GNU Affero General Public License,
section 13, concerning interaction through a network will apply to the
combination as such.
\

14. Revised Versions of this License.
\

The Free Software Foundation may publish revised and/or new versions of
the GNU General Public License from time to time.  Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns.
\

Each version is given a distinguishing version number.  If the
Program specifies that a certain numbered version of the GNU General
Public License \"or any later version\" applies to it, you have the
option of following the terms and conditions either of that numbered
version or of any later version published by the Free Software
Foundation.  If the Program does not specify a version number of the
GNU General Public License, you may choose any version ever published
by the Free Software Foundation.
\

If the Program specifies that a proxy can decide which future
versions of the GNU General Public License can be used, that proxy's
public statement of acceptance of a version permanently authorizes you
to choose that version for the Program.
\

Later license versions may give you additional or different
permissions.  However, no additional obligations are imposed on any
author or copyright holder as a result of your choosing to follow a
later version.
\

15. Disclaimer of Warranty.
\

THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY
APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY
OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM
IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF
ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
\

16. Limitation of Liability.
\

IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS
THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
				     DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
				     PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGES.
\

17. Interpretation of Sections 15 and 16.
\

If the disclaimer of warranty and limitation of liability provided
above cannot be given local legal effect according to their terms,
reviewing courts shall apply local law that most closely approximates
an absolute waiver of all civil liability in connection with the
Program, unless a warranty or assumption of liability accompanies a
copy of the Program in return for a fee.
"]
}

set OtherPopups(HowTo) "";
proc HowTo {} {
    if {[PopupDown HowTo] ==1} {return}
    set po [CreateTextDisplay [_ "How to Use this Program"] 60 15]
    set ::HelpPopups(HowTo) $po;
    AppendToTextDisplay $po [_ "This program provides an interface to two programs that do the actual conversions between Unicode and ASCII. It assembles the necessary information and then runs the appropriate program. What you need to do is to provide information about what you want to do. When you are ready, press the 'Convert' button.\n\n"]
    AppendToTextDisplay $po [_ "The first thing that you need to indicate is the direction of the conversion: from Unicode to ASCII or from ASCII to Unicode.\n\n"]
    AppendToTextDisplay $po [_ "Next, select the file from which the input is to be read and specify the name of the file into which the result of the conversion should be put. You can type the filenames directly into the entry boxes or you can browse by pressing the 'Browse' buttons.\n\n"]
    AppendToTextDisplay $po [_ "Next specify the ASCII format that is used in the input text if you are converting from ASCII to Unicode, or the ASCII format that you want to use if you are converting from Unicode to ASCII. If you select a format with your left mouse button and then click your right mouse button, some additional information about that format will be displayed if there is any.\n\n"]
    AppendToTextDisplay $po [_ "Finally, make any changes that you wish to make in the character replacement options and miscellaneous options and press 'Convert'. If you have made all the necessary choices, U2A will execute the appropriate conversion program and report the result. If you have not yet provided all the necessary information you will be notified of what is missing.\n\n"]
    AppendToTextDisplay $po [format [_ "If you often have to do this same conversion, you may wish to create a shell script rather than using this graphical interface all the time. To assist you in this, the 'Show Command Line' command on the File menu displays the command that would be executed if the 'Convert' button were pressed. Note that the last part of the command line, '2> %s', sends the standard error output to a temporary file. You may not want to do this when running the programs yourself.\n\n"] $::ResultFile];
    AppendToTextDisplay $po [_ "If you become tired of the balloon help, you can disable it using the checkbox on the 'Help' menu."]
}

#The order in which the options are listed here is the order in which they will be
#displayed.
set OptionList {r x f e u v l a p b c g 1 3 d 2 4 5 6 7 8 h n m q y k i j o s t}
set AsciiOptions(1,example) "\#x00E9"
set AsciiOptions(1,description) "Hexadecimal with \#x preceding four digits"
set AsciiOptions(1,info) "This notation is used in Common Lisp."
set AsciiOptions(2,example) "v233"
set AsciiOptions(2,description) "Decimal with v preceding number"
set AsciiOptions(2,info) "This notation is used in Perl."
set AsciiOptions(3,example) "\$00E9"
set AsciiOptions(3,description) "Hexadecimal with \$ preceding number"
set AsciiOptions(3,info) "This notation is used in Pascal and by many assemblers."
set AsciiOptions(a,example) "<U00E9>"
set AsciiOptions(a,description) "hexadecimal with prefix U in angle brackets"
set AsciiOptions(a,info) "This notation is used in POSIX locale specifications."
set AsciiOptions(b,example) "\\x00E9"
set AsciiOptions(b,description) "hexadecimal with prefix \\x"
set AsciiOptions(c,example) "\\x\{00E9\}"
set AsciiOptions(c,description) "hexadecimal in braces with prefix \\x"
set AsciiOptions(c,info) "This notation is used in Perl."
set AsciiOptions(d,example) "&\#0233;"
set AsciiOptions(d,description) "decimal numeric character reference"
set AsciiOptions(d,info)  "This notation is used in HTML"
set AsciiOptions(e,example) "U00E9"
set AsciiOptions(e,description) "hexadecimal with prefix U"
set AsciiOptions(f,example) "u00E9"
set AsciiOptions(f,description) "hexadecimal with prefix u"
set AsciiOptions(g,example) "X\'00E9\'"
set AsciiOptions(g,description) "hexadecimal in single quotes with prefix X"
set AsciiOptions(h,example) "&\#x00E9"
set AsciiOptions(h,description) "hexadecimal numeric character reference"
set AsciiOptions(h,info) "This notation is used in HTML."     
set AsciiOptions(i,example) "=C3=A9"
set AsciiOptions(i,description) "hexadecimal UTF-8 with =-sign prefix"
set AsciiOptions(i,info) "This is the Quoted Printable format defined in RFC2045"
set AsciiOptions(j,example) "%C3%A9"
set AsciiOptions(j,description) "hexadecimal UTF-8 with %-sign prefixes"
set AsciiOptions(j,info) "This is the Universal Resource Indicator notation defined in RFC2396."
set AsciiOptions(k,example) "\\303\\251"
set AsciiOptions(k,description) "octal UTF-8 with backslash prefixes"
set AsciiOptions(l,example) "\\u00E9 / \\U10001"
set AsciiOptions(l,description) "hexadecimal with prefix \\u within BMP, \\U outside"
set AsciiOptions(l,info) "This format is used in Tcl and Scheme source code."
set AsciiOptions(m,example) "\\\#xE9;"
set AsciiOptions(m,description) "hexadecimal with prefix backslash crosshatch x and suffix semi-colon"
set AsciiOptions(m,info) "This format is used in SGML."
set AsciiOptions(n,example) "\\\#233;"
set AsciiOptions(n,description) "decimal with prefix backslash crosshatch x and suffix semi-colon"
set AsciiOptions(n,info) "This format is used in SGML."
set AsciiOptions(o,example) "\\000\\000\\351"
set AsciiOptions(o,description) "The low three bytes in octal in big-endian order."
set AsciiOptions(s,info) "This format is one of three permitted in POSIX portable charmaps."
set AsciiOptions(p,example) "U+00E9"
set AsciiOptions(p,description) "hexadecimal with prefix U+"
set AsciiOptions(p,info) "This is the notation used by the Unicode Consortium"
set AsciiOptions(q,example) "&eacute;"
set AsciiOptions(q,description) "character entity;"
set AsciiOptions(q,info)  "This notation is used in HTML.\nSelecting this conversion in going from ASCII to Unicode\nmeans that only character entities will be converted to Unicode.\nSelecting this conversion in the direction from Unicode\nto ASCII means that character entities will be generated\nwhere possible. Where character entities do not exist,\nhexadecimal character references will be generated."
set AsciiOptions(r,example) "00E9"
set AsciiOptions(r,description) "raw hexadecimal number"
set AsciiOptions(s,example) "\\x00\\x00\\xE9"
set AsciiOptions(s,description) "The low three bytes in hexadecimal in big-endian order."
set AsciiOptions(s,info) "This format is one of three permitted in POSIX portable charmaps."
set AsciiOptions(t,example) "\\d000\\d000\\d233"
set AsciiOptions(t,description) "The low three bytes in decimal in big-endian order."
set AsciiOptions(t,info) "This format is one of three permitted in POSIX portable charmaps."
set AsciiOptions(u,example) "\\u00E9"
set AsciiOptions(u,description) "hexadecimal with prefix \\u"
set AsciiOptions(u,info) "This notation is used in Java and Python source code."
set AsciiOptions(v,example) "\\u00233"
set AsciiOptions(v,description) "decimal with prefix \\u"
set AsciiOptions(v,info) "This notation is used in Rich Text Format (RTF)."
set AsciiOptions(x,example) "0x00E9"
set AsciiOptions(x,description) "standard format hexadecimal number"
set AsciiOptions(y,example) ""
set AsciiOptions(y,description) "all three HTML formats"
set AsciiOptions(4,description) "Generate hexadecimal numbers with prefix 16\#"
set AsciiOptions(4,example) "16\#00E9"
set AsciiOptions(4,info) "This notation is used in Postscript"
set AsciiOptions(5,description)  "Generate hexadecimal numbers with prefix \#16r"
set AsciiOptions(5,example) "\#16r00E9"
set AsciiOptions(5,info) "This notation is used in Common Lisp"
set AsciiOptions(6,description)  "Generate hexadecimal numbers with prefix 16\# and suffix \#"
set AsciiOptions(6,example) "16\#00E9\#"
set AsciiOptions(6,info) "This notation is used in Ada"
set AsciiOptions(7,example) "\\xC3\\xA9"
set AsciiOptions(7,description) "hexadecimal UTF-8 with \\x prefixes"
set AsciiOptions(7,info) "This notation is used in Apache log files"
set AsciiOptions(8,example) "_x00E9_"
set AsciiOptions(8,description) "hexadecimal with _x prefix and _ suffix"
set AsciiOptions(8,info) "This notation is used in Microsoft's OOXML document file format"

set AsciiOptions(Example,info) "This column shows examples of the various types of notation.\nIn most cases the example is of the representation of the\nletter \u00E9 \"e with acute accent\". The exception is the\ncase in which the notation is different within the BMP and\noutside it. Here the first example is of \u00E9 and the second\nis of \"Linear B syllable E\"."
set AsciiOptions(Description,info) "This column contains a description of the notation."

trace add variable WhichWay write ToggleOptionsEnabled
proc ToggleOptionsEnabled {e o n} {
    upvar $e aup;
    if {$aup} {
	$::GENOPTIONS.tbl.ipt configure -state normal
	$::GENOPTIONS.tbl.aws configure -state normal
	$::GENOPTIONS.tbl.hu configure -state disabled
	$::GENOPTIONS.tbl.pt configure -state disabled
	$::GENOPTIONS.tbl.nt configure -state disabled
	$::GENOPTIONS.tbl.wt configure -state disabled
	$::GENOPTIONS.tbl.at configure -state disabled
	$::DOWNOPTIONS.tbl.sty configure -state disabled
	$::DOWNOPTIONS.tbl.cir configure -state disabled
	$::DOWNOPTIONS.tbl.dia configure -state disabled
	$::DOWNOPTIONS.tbl.app configure -state disabled
	$::DOWNOPTIONS.tbl.aps configure -state disabled
	$::DOWNOPTIONS.tbl.exp configure -state disabled
    } else {
	$::GENOPTIONS.tbl.ipt configure -state disabled
	$::GENOPTIONS.tbl.aws configure -state disabled
	$::GENOPTIONS.tbl.hu configure -state normal
	$::GENOPTIONS.tbl.pt configure -state normal
	$::GENOPTIONS.tbl.nt configure -state normal
	$::GENOPTIONS.tbl.wt configure -state normal
	$::GENOPTIONS.tbl.at configure -state normal
	$::DOWNOPTIONS.tbl.sty configure -state normal
	$::DOWNOPTIONS.tbl.cir configure -state normal
	$::DOWNOPTIONS.tbl.dia configure -state normal
	$::DOWNOPTIONS.tbl.app configure -state normal
	$::DOWNOPTIONS.tbl.aps configure -state normal
	$::DOWNOPTIONS.tbl.exp configure -state normal
    }
}

proc ProgramTimeDateStamp {} {
    set sts [split $::TimeStamp]
    return "[lindex $sts 0] [lindex $sts 1]"
}

#Set up balloon help
toplevel .balloonhelp -class Balloonhelp -background black -borderwidth 1 -relief flat
#label .balloonhelp.arrow -anchor nw -bitmap @arrow.xbm
#pack .balloonhelp.arrow -side left -fill y
label .balloonhelp.info -font BalloonHelpFont;
pack .balloonhelp.info -side left -fill y
wm overrideredirect .balloonhelp 1
wm withdraw .balloonhelp
set bhInfo(active) 1

proc balloonhelp_control {state} {
     global bhInfo
     if {$state} {
          set bhInfo(active) 1
     } else {
	balloonhelp_cancel
	set bhInfo(active) 0
     }
}

proc balloonhelp_for {win mesg} {
    global bhInfo
    set bhInfo($win) $mesg
    set ::bhOverlapP($win) 1; 
    bind $win <Enter> {+balloonhelp_pending %W}
    bind $win <Leave> {+balloonhelp_cancel}
}

proc balloonhelpd_for {win mesg} {
    global bhInfo
    set ::bhOverlapP($win) 0;
    set bhInfo($win) $mesg
    bind $win <Enter> {+balloonhelp_show %W}
    bind $win <Leave> {+wm withdraw .balloonhelp}
}

proc balloonhelp_pending {win} {
     global bhInfo
     balloonhelp_cancel
     set bhInfo(pending) [after 1000 [list balloonhelp_show $win]]
}

proc balloonhelp_cancel {} {
    global bhInfo
    if { [info exists bhInfo(pending)]} {
	after cancel $bhInfo(pending)
	unset bhInfo(pending)
    }
    wm withdraw .balloonhelp
}

proc balloonhelp_show {win} {
    global bhInfo;
    global bhOverlapP;
    if {$bhOverlapP($win)} {
	set Overlap 25;
    } else {
	set Overlap -10;
    }
    if {[winfo exists $win]} {
	if {$bhInfo(active)} {
	    .balloonhelp.info configure -text $bhInfo($win)
	    #Set abcissa
	    set MaxStringWidth 0;
	    foreach line [split $bhInfo($win) "\n"] {
		set StringWidth [font measure BalloonHelpFont -displayof .balloonhelp.info $line]
		if {$StringWidth > $MaxStringWidth} {
		    set MaxStringWidth $StringWidth;
		}
	    }
	    set ScreenWidth [winfo screenwidth $win]
	    set Width [winfo width $win];
	    set LeftEdge  [winfo rootx $win];
	    set RightEdge [expr $LeftEdge + $Width];
	    if {$ScreenWidth - $RightEdge < $MaxStringWidth} { 
		if {$LeftEdge > $MaxStringWidth} {
		    set x [expr $LeftEdge - $MaxStringWidth + $Overlap];
		} else {
		    if {$ScreenWidth - $MaxStringWidth > 0} {
			set x [expr $RightEdge - $MaxStringWidth];
		    } else {
			set x [expr $ScreenWidth - $MaxStringWidth];
		    }
		}
	    } else {
		set x [expr $RightEdge - $Overlap];
	    }
	    #Set ordinate
	    set Height [winfo height $win];
	    set TopEdge [winfo rooty $win];
#	    set y [expr $TopEdge + ($Height/2)];
	    set y [expr $TopEdge + int(($Height/1.5))];
	    wm geometry .balloonhelp +$x+$y
	    wm deiconify .balloonhelp
	    raise .balloonhelp
	}
    }
    if {[info exist bhInfo(pending)]} {
	unset bhInfo(pending)
    }
}

proc ToggleBalloonHelp {} {
    global BalloonHelpP;
    global BalloonHelpIndex;
    global m;

    if {$BalloonHelpP} {
	set BalloonHelpP 0;
	balloonhelp_control 0
	ShowMessage [_ "Irritating Balloon Help Disabled"];
	$m.configure entryconfigure $BalloonHelpIndex -label [_ "Show Balloon Help"];
    } else {
	set BalloonHelpP 1;
	balloonhelp_control 1
	ShowMessage [_ "Balloon Help Enabled"];
	$m.configure entryconfigure $BalloonHelpIndex -label [_ "Hide Irritating Balloon Help"];
    }
}

proc ConstructUACommandLine {ind} {
    set cl [list]
    lappend cl uni2ascii
    if {$::ConvertEnclosedP} {
	lappend cl "-c"
    }
    if {$::StripDiacriticsP} {
	lappend cl "-d"
    }
    if {$::ConvertApproximateP} {
	lappend cl "-e"
    }
    if {$::ConvertStyleP} {
	lappend cl "-f"
    }
    if {$::ExpandToAsciiP} {
	lappend cl "-x"
    }
    if {$::ConvertSingleApproximateP} {
	lappend cl "-y"
    }
    if {$::HexUpperCaseP == 0} {
	lappend cl "-l"
    }
    if {$::PreserveNewlinesP == 0} {
	lappend cl "-n"
    }
    if {$::PreserveSpaceP == 0} {
	lappend cl "-s"
    }
    if {$::ConvertAsciiP} {
	lappend cl "-p"
    }
    if {$::AddSpaceP} {
	lappend cl "-w"
    }
    if {$::AcceptWithoutSemicolonP} {
	lappend cl "-m"
    }
    foreach s $::SubstitutionList {
	lappend cl "-S"
	lappend cl [format "%s:%s" [lindex $s 0] [lindex $s 1]]
    }
    foreach d $::DeletionList {
	lappend cl "-S"
	lappend cl [format "%s:" $d]
    }
    set i [lindex $::OptionList $ind]
    lappend cl "-a"
    lappend cl [format "%s" [string toupper $i]]
    if {[string equal $i "q"]} {
	lappend cl "-a H"
    }
    return $cl
}

proc ConstructAUCommandLine {ind} {
    set cl [list]
    lappend cl ascii2uni
    if {$::InputPureP} {
	lappend cl "-p"
    }
    lappend cl [format "\-%s" [string toupper [lindex $::OptionList $ind]]]
    return $cl
}

proc ConstructCommandLine {ind inf outf} {
    if {$::WhichWay} {
	set cl [ConstructAUCommandLine $ind]
    } else {
	set cl [ConstructUACommandLine $ind]
    }
    lappend cl "<"
    lappend cl $inf;
    lappend cl ">"
    lappend cl $outf;
    lappend cl "2>"
    lappend cl $::ResultFile
    return $cl
} 

set PreviousIndex "";
proc ExecuteDaughter {ReallyP} {
    ClearMessage
    set index [$::AFORMATS.f.tl curselection]
    if {[string equal $index ""]} {
	if {[string equal $::PreviousIndex ""]} {
	    ShowMessage "No ASCII format has been selected."
	    return "";
	} else {
	    set index $::PreviousIndex;
	}
    } else {
	set ::PreviousIndex $index;
    }
    set inf [$::INPUT.ent get]
    if {[string equal $inf ""]} {
	ShowMessage "No input file has been specified."
	return "";
    }
    if {[file readable $inf] == 0} {
	ShowMessage "The specified input file is not readable."
	return ""
    }
    set outf [$::OUTPUT.ent get]
    if {[string equal $outf ""]} {
	ShowMessage "No output file has been specified."
	return "";
    }
    if {[file exist $outf] && ([file writable $outf] == 0)} {
	ShowMessage "The specified output file is not writable."
	return ""
    }
    set cl [ConstructCommandLine $index $inf $outf]
    if {$ReallyP} {
	set cl [linsert $cl 0 "exec"]
	set result [eval $cl];
	set rlist [GetResultInfo]
	set Converted [lindex $rlist 0]
	set msg [format "tokens converted: %d" $Converted]
	if {$::WhichWay} {		# Ascii to unicode
	    set MicrosoftStyle [lindex $rlist 2]
	    append msg [format " (Microsoft-style %d)" $MicrosoftStyle]
	    set Replaced  [lindex $rlist 1]
	    append msg [format "  replaced: %d" $Replaced]
	} else {
	    set TotalChars [lindex $rlist 1]
	    append msg [format " out of %d total characters" $TotalChars]
	}
	ShowMessage $msg
    	return $result;
    } else {
	ShowMessage $cl;
    }
}

#Return a list of three values: number of tokens converted, number replaced, number
#of Microsoft-style non-standard HTML.
proc GetResultInfo {} {
    if {[catch {open $::ResultFile "r"} ResultHandle ] != 0} {
	ShowMessage [format [_ "Mysterious failure to open temporary file %s."] $::ResultFile];
	return "";
    }
    #Read first line of log
    if {[gets $ResultHandle line] <= 0} {
	return [list 0 0 0]
    }
    set rlist [split $line]
    lappend res [lindex $rlist 0]
    if {!$::WhichWay} {
	lappend res [lindex $line 5]
    }
    #Attempt to read second line of log, which will contain replacement count, if ascii2uni,
    #or the number of input characters, if uni2ascii.
    if {[gets $ResultHandle line] > 0} {
	lappend res [lindex $line 0]
    } else {
	lappend res 0
	if {$::WhichWay} {
	    lappend res 0
	}
	return $res
    }
    if {[gets $ResultHandle line] > 0} {
	lappend res [lindex $line 0]
    } else {
	if {$::WhichWay} {
	    lappend res 0
	}
    }
    return $res;
}

proc PopupInfo {o} {
    if {[info exist ::AsciiOptions($o,info)]} {
	set info $::AsciiOptions($o,info);
	if {[PopupDown $o] ==1} {return}
	set po [CreateTextDisplay $o 72 12]
	set ::HelpPopups($o) $po
	AppendToTextDisplay $po $info;
    }
}

proc CellInfo {} {
    set row [lindex  [split [$::AFORMATS.f.tl curcellselection] ","] 0]
    PopupInfo [lindex $::OptionList $row]
}

#If the filename passed as argument is a pathname
#leading to a file in the current working directory,
#return just the basename+extension. Otherwise
#return the argument.
proc MinimizeFileName {s} {
    set cwd [pwd];
    set sdir [file dirname $s]
    if {[string equal $cwd $sdir]} {
	return [file tail $s]
    } else {
	return $s;
    }
}

proc SelectInputFile {} {
    set InputFile [tk_getOpenFile];
    if {$InputFile == ""} {
	ShowMessage [_ "File selection aborted."]
    } else { 
	$::INPUT.ent delete 0 end
	$::INPUT.ent insert 0 [MinimizeFileName $InputFile]
    }
}

proc SelectOutputFile {} {
    set OutputFile [tk_getSaveFile -initialfile [_ "MinpairOutput"]];
    if {$OutputFile == ""} {
	ShowMessage [_ "File selection aborted."]
    } else { 
	$::OUTPUT.ent delete 0 end
	$::OUTPUT.ent insert 0 [MinimizeFileName $OutputFile]
    }
}

proc DescribeColumn {w c} {
    if {$c == 0} {
	PopupInfo Description;
    } else {
	PopupInfo Example;
    }
}

puts "u2a $Version";
puts "Copyright (C) 2005-2008 William J. Poser.";
puts [_ "This program is free software; you can redistribute it
and/or modify it under the terms of version 2 of the GNU General
Public License as published by the Free Software Foundation."];

set m [menu .menubar -tearoff 0 -font MenuFont \
       -bg $ColorSpecs(Menubar,Background) \
       -fg $ColorSpecs(Menubar,Foreground) \
       -activebackground $ColorSpecs(Menubar,ActiveBackground)\
       -activeforeground $ColorSpecs(Menubar,ActiveForeground)]
set MenuBarItemCnt -1;
$m add cascade -label [_ "File"]   -menu [menu $m.file]
$m.file add command -label [_ "Show Command Line"] -command {ExecuteDaughter 0}
$m.file add command -label [_ "Quit"] -command ShutDown;
$m add cascade -label [_ "Help"]   -menu [menu $m.help]
$m.help add command -label "About" -command About
$m.help add checkbutton -label "Balloon Help" -variable BalloonHelpP \
    -onvalue 1 -offvalue 0 -command {balloonhelp_control $::BalloonHelpP} -indicatoron 1 \
    -selectcolor coral
$m.help add command -label "Bug Reports" -command BugReports
$m.help add command -label "How to Use this Program" -command HowTo
$m.help add command -label "License" -command ShowGPL
$m.help add command -label "Unicode Consortium Web Site" -command {ShowWebPage http://www.unicode.org}
. configure -menu .menubar

frame .mf -background $ColorSpecs(MainFrame,Background)
pack .mf

text $MSG -bg $ColorSpecs(Messages,Background) \
    -fg $ColorSpecs(Messages,Foreground) \
    -height 1 -width 60\
    -relief sunken -font MainFont -exportselection 1 -state disabled

balloonhelp_for $MSG "Messages from the program appear here."

set WHICHWAY .mf.whichway
frame $WHICHWAY -border 2 -relief ridge
frame $WHICHWAY.f -border 2 -relief ridge
label $WHICHWAY.f.tit -text "Direction of Conversion"
radiobutton $WHICHWAY.f.tou -variable WhichWay -value 1 -text "ASCII to Unicode"
radiobutton $WHICHWAY.f.toa -variable WhichWay -value 0 -text "Unicode to ASCII"
button $WHICHWAY.exe -text "Convert" -command {ExecuteDaughter 1} \
    -bg "\#FF9f8c" -fg black -activeforeground black -activebackground red
pack $WHICHWAY.f.tit -side top -expand 0 -fill none -padx 2 -pady {4 5} -anchor w
pack $WHICHWAY.f.tou -side left -expand 1 -fill both -padx {2 4} -pady 5
pack $WHICHWAY.f.toa -side left -expand 1 -fill both -padx {4 5} -pady 5
pack $WHICHWAY.f   -side left -expand 1 -fill both -padx {5  40} -pady {6 6}
pack $WHICHWAY.exe -side left -expand 1 -fill both -padx {40 10} -pady 6

set msg "Here is where you choose whether to convert from Unicode to an
ASCII representation or from an ASCII representation to Unicode."
balloonhelp_for $WHICHWAY $msg

set msg "When you have set everything up and are ready to
actually carry out the conversion, press this button."
balloonhelp_for $WHICHWAY.exe $msg

set GENOPTIONS .mf.f
frame $GENOPTIONS -border 2 -relief ridge
label $GENOPTIONS.tit -text "Miscellaneous Options"
frame $GENOPTIONS.tbl
checkbutton $GENOPTIONS.tbl.hu -variable HexUpperCaseP -text "Upper case hexadecimal numbers" \
    -indicatoron 0
checkbutton $GENOPTIONS.tbl.pt -variable ConvertAsciiP  -offvalue 0 -onvalue 1 \
    -text "Convert ASCII characters"   -indicatoron 0
checkbutton $GENOPTIONS.tbl.nt -variable PreserveNewlinesP  -offvalue 1 -onvalue 0 \
    -text "Convert linefeed characters"     -indicatoron 0
checkbutton $GENOPTIONS.tbl.wt -variable PreserveSpaceP  -offvalue 1 -onvalue 0 \
    -text "Convert space characters"     -indicatoron 0
checkbutton $GENOPTIONS.tbl.at -variable AddSpaceP -text "Add a space after each item" \
       -indicatoron 0
checkbutton $GENOPTIONS.tbl.ipt -variable InputPureP -text "Input is pure" \
    -indicatoron 0
checkbutton $GENOPTIONS.tbl.aws -variable AcceptWithoutSemicolonP -text "No Semicolon OK?" \
    -indicatoron 0

set msg "Here is where you can set various options. Note that some options
apply only to uni2ascii and others only to ascii2uni. The options
that are not relevant to the current direction of conversion are
disabled and appear greyed out."
balloonhelp_for $GENOPTIONS $msg

set msg "Select this option if you want to use the upper case
letters A, B, C, D, E, and F in hexadecimal numbers,
where they represent the numbers 10, 11, 12, 13, 14, and 15.
For example, the letter \u00E9, e with acute accent, will
look like this in standard hexadecimal notation: 0x00E9 
If you do not select it, the letters a, b, c, d, e, and f
will be used in hexadecimal numbers, where they represent
the numbers 10, 11, 12, 13, 14, and 15.
For example, the letter \u00E9, e with acute accent, will
look like this in standard hexadecimal notation: 0x00e9 
This is usually an aesthetic choice, but some programs care
about case."
balloonhelp_for $GENOPTIONS.tbl.hu $msg

set msg "Select this option if you want to convert all
characters in the input, even ASCII characters (those
at codepoints less than 0x0080) to a textual
representation. For example, if the input contains
the letter \'a\', it will be converted to a textual
representation such as the standard hexadecimal 0x0061.
For example, the French word \u00e9\u0074\u00e9 \'summer\' will look like:
0x00E9 0x0074 0x00E9
(with spaces inserted to make it easier to parse).

If this option is selected ASCII characters other than
space (U+0020), tab (U+0009), and newline (U+000A) will
be converted. The conversion of space, tab, and newline
is controlled by other options.

Do not select this option if you want to preserve ASCII
characters (those at codepoints less than 0x0080) rather
than converting them to textual representation. If the input
text contains a mixture of ASCII and non-ASCII characters,
the output will contain a mixture of ASCII characters and
textual representations. For example, the French word \u00e9\u0074\u00e9 \'summer\'
will look like:
0x00E9 t 0x00E9
(with spaces inserted to make it easier to parse)."
balloonhelp_for $GENOPTIONS.tbl.pt $msg

set msg "Select this option if you wish to convert
linefeed characters (U+000A) to textual
representations. Note that unless you select
this option the space and tab characters will
not be converted even if you have chosen to
convert ASCII characters in general."
balloonhelp_for $GENOPTIONS.tbl.nt $msg

set msg "Select this option if you wish to convert the whitespace
characters space (U+0020), tab (U+0009), ethiopic word
space (U+1361), ogham space (U+1680), and ideographic
space (U+3000)  to textual representation. Note that
unless you select this option the space and tab characters
will not be converted even if you have chosen to
convert ASCII characters in general.

Do not select this option if you do not wish to convert
whitespace characters to textual representation.
If this option is not chosen, the characters space
(U+0020) and  tab (U+0009) will not be converted
even if other ASCII characters are.

The characters ethiopic word space (U+1361),
ogham space (U+1680), and ideographic space
(U+3000) will not be converted into their
textual representations but will be converted
to the ASCII space character (U+0020) in order
to make the output pure ASCII.

Leaving whitespace unconverted preserves word
boundaries and the like which is helpful if
you need to visually inspect the text."
balloonhelp_for $GENOPTIONS.tbl.wt $msg

set msg "Select this option to add a space after the ASCII representation
of each character. If you are converting Unicode to ASCII in order
to inspect it, it is generally easier to parse this way.
Typical output will look like this:
0x0074 0x0065 0x0078 0x0074
Leave this option unselected if you want the ASCII representations of
the Unicode to come one right after the other. Typical output
will look like this:
0x00740x00650x00780x0074"
balloonhelp_for $GENOPTIONS.tbl.at $msg

set msg "Select this option if the input text consists entirely
of escapes, separated by whitespace,  with no regular text.
For example, if the Unicode is represented by canonical
hexadecimal numbers, it might look like this:
0x0074 0x0065 0x0078 0x0074
After conversion it would look like this:
text

Leave this option unselected if the input text consists of a mixture
of escapes and regular text. For example, if the Unicode is
represented by canonical hexadecimal numbers, the input text
might look like this:
the symbol 0x0E3F represents the baht, the Thai unit of currency
After conversion it would look like this:
the symbol \u0E3F represents the baht, the Thai unit of currency."
balloonhelp_for $GENOPTIONS.tbl.ipt $msg


set OXP 4
set OYP 2
grid $GENOPTIONS.tbl.pt -row 0 -column 0 -sticky w -padx $OXP -pady $OYP
grid $GENOPTIONS.tbl.nt -row 0 -column 1 -sticky w -padx $OXP -pady $OYP
grid $GENOPTIONS.tbl.wt -row 0 -column 2 -sticky w -padx $OXP -pady $OYP
grid $GENOPTIONS.tbl.at -row 1 -column 0 -sticky w -padx $OXP -pady $OYP
grid $GENOPTIONS.tbl.hu -row 1 -column 1 -sticky w -padx $OXP -pady $OYP
grid $GENOPTIONS.tbl.aws -row 1 -column 2 -sticky w -padx $OXP -pady $OYP
grid $GENOPTIONS.tbl.ipt -row 1 -column 3 -sticky w -padx $OXP -pady $OYP
pack $GENOPTIONS.tit -side top -expand 0 -fill none  -padx 2 -pady {4 2} -anchor w
pack $GENOPTIONS.tbl -side top -expand 1 -fill both  -padx {6 2} -pady {4 6} -anchor w

set DOWNOPTIONS .mf.d
frame $DOWNOPTIONS -border 2 -relief ridge
label $DOWNOPTIONS.tit -text "Character Replacement Options"
frame $DOWNOPTIONS.tbl
checkbutton $DOWNOPTIONS.tbl.sty -text [_ "Stylistic Equivalents"] -variable ConvertStyleP \
    -indicatoron 0
checkbutton $DOWNOPTIONS.tbl.cir -text [_ "Remove Enclosures"]  -variable ConvertEnclosedP \
    -indicatoron 0
checkbutton $DOWNOPTIONS.tbl.dia -text [_ "Strip Diacritics"]   -variable StripDiacriticsP \
    -indicatoron 0
checkbutton $DOWNOPTIONS.tbl.app -text [_ "Approximate"] -variable ConvertApproximateP \
    -indicatoron 0
checkbutton $DOWNOPTIONS.tbl.aps -text [_ "Approximate Single"] -variable ApproximateSingleP \
    -indicatoron 0
checkbutton $DOWNOPTIONS.tbl.exp -text [_ "Expand"] -variable ExpandToAsciiP \
    -indicatoron 0
pack $DOWNOPTIONS.tbl.sty $DOWNOPTIONS.tbl.dia  $DOWNOPTIONS.tbl.cir  $DOWNOPTIONS.tbl.app $DOWNOPTIONS.tbl.exp $DOWNOPTIONS.tbl.aps -side left -expand 1 -fill both -padx 6
pack $DOWNOPTIONS.tit -side top -expand 0 -fill none  -padx 2 -pady {4 2} -anchor w
pack $DOWNOPTIONS.tbl -side top -expand 1 -fill both  -padx 2 -pady {4 6}
bind $DOWNOPTIONS.tbl.app <<B3>> ExplainEquivalences
bind $DOWNOPTIONS.tbl.aps <<B3>> ExplainSingleApproximations
bind $DOWNOPTIONS.tbl.exp <<B3>> ExplainExpansions
set msg "If this option is chosen, characters differing from ASCII characters only
in style, such as bold, italic, full width, and fraktur characters,
are converted to the corresponding ASCII character. "
balloonhelp_for $DOWNOPTIONS.tbl.sty $msg
set msg "If this option is chosen, characters enclosed in circles or parentheses
are converted to their plain equivalents."
balloonhelp_for $DOWNOPTIONS.tbl.cir $msg
set msg "If this option is chosen, combining diacritics are deleted and characters
with intrinsic diacritics are replaced with their plain ASCII equivalents."
balloonhelp_for $DOWNOPTIONS.tbl.dia $msg
set msg "If this option is chosen, characters are replaced with approximate ASCII
equivalents. Right-click for the complete list."
balloonhelp_for $DOWNOPTIONS.tbl.app $msg
set msg "If this option is chosen, certain characters are expanded to sequences of
approximately equivalent plain ASCII characters. The expansions are:
\tU+00A2 \u00A2\tCENT SIGN                 \t\u2192 cent
\tU+00A3 \u00A3\tPOUND SIGN                \t\u2192 pound
\tU+00A5 \u00A5\tYEN SIGN                  \t\u2192 yen
\tU+00A9 \u00A9\tCOPYRIGHT SYMBOL          \t\u2192 (c)
\tU+00DF \u00DF \tSMALL LETTER SHARP S     \t\u2192 ss
\tU+00C6 \u00C6\tCAPITAL LETTER ASH        \t\u2192 AE
\tU+00E6 \u00E6 \tSMALL LETTER ASH         \t\u2192 ae
\tU+01F1 \u01F1\tCAPITAL LETTER DZ         \t\u2192 DZ
\tU+01F3 \u01F3\tSMALLL LETTER DZ          \t\u2192 dz
\tU+02A6 \u02A6 \tSMALLL LETTER TS DIGRAPH \t\u2192 ts
\tU+2026 \u2026 \tHORIZONTAL ELLIPSIS      \t\u2192 ...
\tU+20AC \u20AC\tEURO SIGN                 \t\u2192 euro
\tU+2190 \u2190 \tLEFTWARDS ARROW          \t\u2192 <-
\tU+2192 \u2192 \tRIGHTWARDS ARROW         \t\u2192 ->
\tU+21D0 \u21D0 \tLEFTWARDS DOUBLE ARROW   \t\u2192 <=
\tU+21D2 \u21D2 \tRIGHTWARDS DOUBLE ARROW  \t\u2192 =>
\tU+22EF \u22EF \tMIDLINE HORIZONTAL ELLIPSIS\t\u2192 ..."
balloonhelp_for $DOWNOPTIONS.tbl.exp $msg

set msg "If this option is chosen, certain characters are converted to
a single approximately equivalent plain ASCII character. The conversions are:
\tU+00A2 \u00A2\tCENT SIGN                 \t\u2192 C
\tU+00A3 \u00A3\tPOUND SIGN                \t\u2192 \#
\tU+00A5 \u00A5\tYEN SIGN                  \t\u2192 Y
\tU+00A9 \u00A9\tCOPYRIGHT SYMBOL          \t\u2192 C
\tU+00DF \u00DF \tSMALL LETTER SHARP S     \t\u2192 s
\tU+00C6 \u00C6\tCAPITAL LETTER ASH        \t\u2192 A
\tU+00E6 \u00E6 \tSMALL LETTER ASH         \t\u2192 a
\tU+01F1 \u01F1\tCAPITAL LETTER DZ         \t\u2192 D
\tU+01F3 \u01F3\tSMALLL LETTER DZ          \t\u2192 d
\tU+02A6 \u02A6 \tSMALLL LETTER TS DIGRAPH \t\u2192 t
\tU+2026 \u2026 \tHORIZONTAL ELLIPSIS      \t\u2192 .
\tU+20AC \u20AC\tEURO SIGN                 \t\u2192 E
\tU+2190 \u2190 \tLEFTWARDS ARROW          \t\u2192 <
\tU+2192 \u2192 \tRIGHTWARDS ARROW         \t\u2192 >
\tU+21D0 \u21D0 \tLEFTWARDS DOUBLE ARROW   \t\u2192 <
\tU+21D2 \u21D2 \tRIGHTWARDS DOUBLE ARROW  \t\u2192 >
\tU+22EF \u22EF \tMIDLINE HORIZONTAL ELLIPSIS\t\u2192 ."
balloonhelp_for $DOWNOPTIONS.tbl.aps $msg

set AFORMATS .mf.aformats
frame $AFORMATS -border 2 -relief ridge
label $AFORMATS.tit -text "ASCII Format"
frame $AFORMATS.f
tablelist::tablelist $AFORMATS.f.tl -columns {0 "Description" 0 "Example"} \
    -stretch all -background white -width 60 -height 5 -yscrollcommand {$::AFORMATS.f.sb set} \
    -labelbackground $ColorSpecs(ListHeader,Background) \
    -labelforeground $ColorSpecs(ListHeader,Foreground) \
    -labelactivebackground $ColorSpecs(ListHeader,ActiveBackground) \
    -labelactiveforeground $ColorSpecs(ListHeader,ActiveForeground) \
    -labelcommand DescribeColumn \
    -selectbackground $ColorSpecs(TableList,SelectBackground) \
    -selectforeground $ColorSpecs(TableList,SelectForeground)
$AFORMATS.f.tl columnconfigure 0 -labelalign left
$AFORMATS.f.tl columnconfigure 1 -labelalign left
scrollbar $AFORMATS.f.sb -command {$::AFORMATS.f.tl yview} -activebackground red
pack $AFORMATS.tit -side top -expand 0 -fill none -padx 2 -pady {4 2} -anchor w
pack $AFORMATS.f.sb -side right -expand 0 -fill y    -anchor w
pack $AFORMATS.f.tl -side left  -expand 1 -fill both -anchor w
pack $AFORMATS.f   -side top -expand 1 -fill both -padx 2 -pady {2 4}
bind [$AFORMATS.f.tl bodytag] <<B3>> {+CellInfo}
set cnt 0
foreach o $OptionList {
    $AFORMATS.f.tl insert end \
	[list $AsciiOptions($o,description) $AsciiOptions($o,example)]
    if {$cnt % 2} {
	$AFORMATS.f.tl rowconfigure $cnt -background $ColorSpecs(AsciiOptionEven,Background)
    } else {
	$AFORMATS.f.tl rowconfigure $cnt -background $ColorSpecs(AsciiOptionOdd,Background)
    }
    incr cnt;
}

set msg "Here is where you select the kind of textual
representation that you want to convert to or from."
balloonhelp_for $AFORMATS $msg

set INPUT  .mf.inf
set OUTPUT .mf.ouf
set BPAD   .mf.bpad

#Name of input file
frame  $INPUT -relief ridge -border 1
label  $INPUT.title  -text [_ "Input File"]   -anchor w
set btxt [_ "Browse"]
set blen [string length $btxt];
button $INPUT.brb  -text $btxt -width $blen -anchor w -command SelectInputFile \
    -activeforeground $ColorSpecs(Button,ActiveForeground) \
    -activebackground $ColorSpecs(Button,ActiveBackground)
entry  $INPUT.ent  -foreground $::ColorSpecs(UserTextEntry,Foreground) \
    -background $ColorSpecs(UserTextEntry,Background)\
    -font MainFont -width 30

set ifypd 3
pack $INPUT.title -side top -expand y -fill x -anchor w -pady $ifypd
pack $INPUT.brb  -expand 0 -fill none -anchor w -side left -padx 4 -pady $ifypd
pack $INPUT.ent  -expand 1 -fill x  -anchor w -side left -pady $ifypd -padx 3
set bhmsg [_ "Specify the name of the input file.
You can type the name in the entry
box or choose it interactively by
pressing the Browse button."];
balloonhelp_for $INPUT $bhmsg;
balloonhelp_for $INPUT.title $bhmsg;
balloonhelp_for $INPUT.ent [_ "Input will be read from the file whose name is shown in\nthis entry box. You may enter its name directly or\npress the Browse button and use the file selection dialogue."];
balloonhelp_for $INPUT.brb [_ "Press this button to select the name of the input file."]

#Name of output file
frame  $OUTPUT -relief ridge -border 1
label  $OUTPUT.title  -text [_ "Output File"]   -anchor w
button $OUTPUT.brb  -text $btxt -width $blen -anchor w -command SelectOutputFile \
    -activeforeground $ColorSpecs(Button,ActiveForeground) \
    -activebackground $ColorSpecs(Button,ActiveBackground)
entry  $OUTPUT.ent -foreground $::ColorSpecs(UserTextEntry,Foreground) \
    -background $ColorSpecs(UserTextEntry,Background)\
    -font MainFont -width 30
set ofypd 3
pack $OUTPUT.title -side top -expand y -fill x -anchor w  -pady $ofypd
pack $OUTPUT.brb  -expand 0 -fill none -anchor w -side left -padx 4  -pady $ofypd
pack $OUTPUT.ent  -expand 1 -fill x  -anchor w -side left -pady $ofypd -padx 3
set bhmsg [_ "Specify the name of the output file.
You can type the name in the entry
box or choose it interactively by
pressing the Browse button."];
balloonhelp_for $OUTPUT $bhmsg;
balloonhelp_for $OUTPUT.title $bhmsg;
balloonhelp_for $OUTPUT.ent [_ "Output will be written to the file whose name is shown in\nthis entry box. You may enter its name directly or\npress the Browse button and use the file selection dialogue."];
balloonhelp_for $OUTPUT.brb [_ "Press this button to select the name of the output file."]

set XP 8
set YP 3
pack $MSG        -side top -expand 1 -fill both -padx $XP -pady $YP  -anchor w
pack $WHICHWAY   -side top -expand 1 -fill both -padx $XP -pady $YP  -anchor w
pack $INPUT      -side top -expand 1 -fill both -padx $XP -pady $YP  -anchor w
pack $OUTPUT     -side top -expand 1 -fill both -padx $XP -pady $YP  -anchor w
pack $AFORMATS   -side top -expand 1 -fill both -padx $XP -pady $YP -anchor w
pack $DOWNOPTIONS -side top -expand 1 -fill both -padx $XP -pady $YP -anchor w
pack $GENOPTIONS -side top -expand 1 -fill both -padx $XP -pady [list $YP 8] -anchor w
set WhichWay $WhichWay
wm title . [format [_ "Unicode/Ascii Converter %s \[%s\]"] $Version [ProgramTimeDateStamp]]
#If the default browser is on the list, remove it.
set di [lsearch -exact $BrowserList $DefaultBrowser]
if {$di >= 0} {
    set BrowserList [lreplace $BrowserList $di $di]
}
#Add the default browser to the beginning of the list.
set BrowserList [linsert $BrowserList 0 $DefaultBrowser];
