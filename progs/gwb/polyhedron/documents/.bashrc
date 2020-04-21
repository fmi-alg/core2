# base-files version 5.0-3
#################################################################
# file: _bashrc_ubuntu
# (Jul 2009): adapted from _bashrc_cygwin 
# 	(1) Problem: my home in Apple Air is "/Users/cheeyap" not "/home/yap"
#		Soln: change ~yap to ~cheeyap below.
#  Chee (Nov 2004): Use this for all my SOLARIS+CYGWIN bash
#	-- introduce my own color scheme for listing files
#		(taken from /etc/DIR_COLORS)
# 	Test if the variable $_YAP_CYGWIN is set to distinguish the
#	two environments.  This variable is only set by cygwin.bat.
#################################################################
# Mar'07: if we directly use bash (as in new linux) this distinction
#	between cygwin and linux box is not no longer needed:

# FIRST THING: recognize your OS!
if [ `uname` == "Linux" ] 
then
	#echo OS is Linux
	export OS=Linux # in solaris, one gets OSTYPE=solaris
	export myOS=Linux
	_KAI_UBUNTU=1		# some files tests if _YAP_MAC is set
else
	echo Error! Unexpected OS!
fi

if [ $_KAI_UBUNTU ]
then
	echo Reading ~/.bashrc for Ubuntu 
fi

#################################################################
# Mar 28, 2007
#   Decided to avoid the convoluted "tcsh calling bash" approach
#	That caused problems when I tried to do scp, for instance
#	Andrew (helpdexk) suggested this to get CIMS defaults: 
if [ -r /usr/local/lib/.system.bashrc ] ; then
	. /usr/local/lib/.system.bashrc
fi
#################################################################

#################################################################
# PATHs
#	Jan'11: added /usr/texbin to find tex related software from MacTex
#################################################################
	export PATH=~/bin:/usr/texbin:/usr/local/bin:`printenv PATH`
	export LD_LIBRARY_PATH=.:/usr/local/lib:/home/exact/usr/yap/coredev/beta/lib/
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/unsupported/packages/opengl/Mesa/lib
  
export PATH=$PATH:~/Documents/core-2.1.1/gmp/lib/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$PATH

export PATH=$PATH:~/Documents/core-2.1.1/mpfr/lib/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$PATH
#################################################################
# See man bash for more options...
#################################################################
  # Don't wait for job termination notification
   set -o notify

  # Don't use ^D to exit
   set -o ignoreeof

  # Don't put duplicate lines in the history.
   export HISTCONTROL=ignoredups

#################################################################
# File Listing Colors
# Copied from /etc/profile and /etc/DIR_COLORS
# 	eval `dircolors -b /etc/DIR_COLORS`
#  (no dircolors in Mac OS)
#################################################################
#if [ $_YAP_MAC  ]
#	eval `dircolors -b ~/.bash_DIR_COLORS`
#	alias altcolor='eval `dircolors -b ~/.bash_DIR_COLORS`'
#fi

#################################################################
# Aliases (alphabetical)
#################################################################
	# set back space as del:
	alias a2ps8='a2ps -Pnhp8 -1'
	alias a8='a2ps -Pnhp8 -1'
	alias a2ps1='a2ps -1'
	alias a1='a2ps -Pnhp8 -1'
	alias bs='stty erase "^H"'
	alias cpmake='cp ~/bin/Makefile-inc Makefile'
	alias h='history'
	# --color works for gnome:
	alias l='ls -CF --color=auto'
	alias la='ls -A'
	alias la='ls -a'
	alias ld='ls -lF | grep "/"'	# list only directories
	alias less='less -r'
	alias ll='ls -l'
	alias lpq8='lpq -Pnhp8'
	alias lpq23='lpq -Pnhp23'
	alias lpq28='lpq -Pnhp28'
	# bad for sun and ssh, but ok for linux:
	# alias ls='ls -sF --color=tty'
	# alias ls='ls -sF' 	#solaris
	alias ls='ls -sFG' 	#mac
	alias lt='ls -lt'
	alias mmake="make -f ~/bin/Makefile-inc $*"
	alias setls='alias ls="ls -sF --color=tty"'
	alias po='popd'
	alias pu='pushd'
	# alias pwd='echo $cwd' #:prints nothing in bash!
	alias rm='rm -i'
	# rmtex can take a directory path, but it must end with "/", e.g., rmtex c1/
	# alias rmtex='"rm" {!*}*.{dvi,aux,log,blg}'
	# alias rmtex='"rm" *.{dvi,aux,log,blg,out} *~ *.bak *.toc t.{nav,out,snm} *.cb'
	alias rmtex='make -f ~/bin/rm-tex'
	alias rrm='"rm" -f'
	alias sobashrc='source ~/.bashrc'
	alias socshrc='source ~/.cshrc'
	alias whence='type -a'
	#alias uuei='uuencode'
	#alias uud='uudecode'
	#################################################################
	# tmp files (NOT YET TESTED! 2/12/2005)
	alias mv2tmp='chmod a+rw $1; mv $1 ~/tmp/tmp'
	alias mv4tmp='mv ~/tmp/tmp/$1 .'

#################################################################
# software
#################################################################
	alias openof='open -n /Applications/OpenOffice.org.app/'

#################################################################
# viewing documents
#################################################################
# does not work:
#alias mac='acroread.command $* &'
#alias ac='acroread $1 &'

#################################################################
# SEARCHING
#################################################################
	# These aliases allows me to search from ANY directory (but non-recursive).
	# MUST FIX: findtel, findbib, etc are ALSO shell scripts in ~/bin!!!
	#   USAGE:  e.g., findgrep ss=Descartes ig= [list]
	# where "list" is optional target (only to list files where match is found).
	alias findbib='make -f ~/bin/make.bib $*'
	alias findfid='make -f ~/bin/make.fid $*'
	alias findfile='make -f ~/bin/make.findfile $*'
	alias findgrep='make -f ~/bin/make.findgrep $*'
	alias findpap='make -f ~/bin/make.pap $*'
	alias findprob='make -f ~/bin/make.prob $*'
	alias findrem='make -f ~/bin/make.findrem $*'
	alias svnrem='make -f ~/bin/make.svnrem $*'
	alias findtel='make -f ~/bin/make.tel $*'
	alias findtex='make -f ~/bin/make.tex $*'

#################################################################
# vi related:
#################################################################
alias dummy=  # to prevent folding error by gvim!
	
	# (7/09) To make MacVim callable from command windows in OS X, you must:
	#	(1) link from ~/bin/mvim to where MacVim is installed:
	#		> ln -s /Applications/MacVim-7_2-stable-1_2/mvim
	#	(2) Tell MacVim where to find MacVim.app:
	#
	#	(3) In ~/bin/mvim and ~/bin/gvim is linked directly
	#		to /Applications/MacVim.app/mvim.
	#
	#	You can now invoke "mvim" directly!
		export VIM_APP_DIR=/Applications
		export VIM=~/.vim	# dir of user files (eg,.vimrc)
		export myEDITOR=gvim	# using this global variable allows you to 
					# share Makefiles with others (e.g., Gert uses
					# EMACS, so our shared Makefile check this to
					# set the editor.  NOTE: I use "myEDITOR", not
					# "EDITOR" because some other process seems to
					# set "EDITOR" to /Users/cheeyap/bin/mvim...
		#export VIMRUNTIME=...	macOS will figure out # dir of support...
						# ...files (eg, docs)
		alias vibashrc='gvim ~/.bashrc &'
		alias vibeamer='gvim ~/mac/cyg_texmf/{myBeamer,myMac-beamer}.sty &'
		alias vifid='gvim ~/gen/gen/fid/positions-new &'
		alias vimac='gvim ~/mac/cyg_texmf/myMac.sty &'
		alias viphone='gvim ~/gen/gen/info/phones_all &'
		alias viposition='gvim ~/gen/gen/fid/positions-new &'
	        alias vipub='gvim ~/ref/ref/yap.bib ~/prop/cv/cvpubRev.tex &'
		alias vitest='gvim ~/tmp/myMac.sty" &'
		alias vivarious='gvim ~/gen/gen/info/various &'
		alias vivimrc='gvim ~/.vimrc &'
#################################################################
# SVN -- needed first, to set up my environment locally
#################################################################
	export SVNROOT=https://yap@subversive.cims.nyu.edu/exact
	#export SVN_EDITOR=gvim -- does not work...
	export SVN_EDITOR=vi
	alias svncom='svn commit -m "yap-macOS:svncom" '
	alias svnmore='svn info; ls .svn/text-base'
	alias svnup='svn update'
	alias svnpath='printenv | grep SVN'
	#alias svnyap='export SVNROOT=https://yap@subversive.cims.nyu.edu/yap'
	alias svnyap='export SVNROOT=svn+ssh://yap@access.cims.nyu.edu/home/yap/svn'
	alias svnexact='export SVNROOT=https://exact@subversive.cims.nyu.edu/exact'
	alias svnrob='export SVNROOT=https://exact@subversive.cims.nyu.edu/exact/robotics'

alias dummy=  # to prevent folding error by gvim!
#################################################################
# CVS and SVN -- needed first, to set up my environment locally
#################################################################
	alias dummy=  # to prevent folding error by gvim!
	export CVSROOT=:ext:yap@access.cims.nyu.edu:/home/yap/cvsroot
	# export CVSROOT=:ext:exact@access.cims.nyu.edu:/home/exact/cvsroot
	export CVS_RSH=ssh
	# NOTE: if CVSEDITOR=gvim gives trouble (MacOS and Cygwin), switch to "vim"
	export CVSEDITOR=gvim
	export EDITOR=gvim
	alias cvsmore='more CVS/Root; more CVS/Repository; more CVS/Entries'
	# want "cvsmore" to take a path argument...
	# does not work:
	#alias cmore='more \$1/CVS/Root; more \$1/CVS/Repository; more $1/CVS/Entries'
	# what about:
	# alias cmore='more \!^/CVS/Root; more \!^/CVS/Repository; more \!^/CVS/Entries'
	# this works:
	cm () { more $*/CVS/Root; more $*/CVS/Repository; more $*/CVS/Entries; }
	scom () { cd ~/mac/mac; svn commit -m "yap-macOS:ccm"; \
	    cd ~/ref/ref; svn commit -m "yap-macOS:ccm";}
	alias cvscom='cvs com -m "yap-macOS:cvscom" '
	alias cvscominfo='pu ~/gen/gen/info; cvs com -m "yap-macOS:cvscominfo"; po '
	alias cvscomtex='pu ~/mac/mac; cvs com -m "yap-macOS:cvscomtex"; cd ~/ref/ref; cvs com -m "yap-macOS:cvscomtex"; po '
	alias cvsup='cvs up'
	alias cvspath='printenv | grep CVS'
	alias cvsyap='export CVSROOT=:ext:yap@access.cims.nyu.edu:/home/yap/cvsroot'
	alias cvsexact='export CVSROOT=:ext:exact@access.cims.nyu.edu:/home/exact/cvsroot'
	alias cvsvisual='export CVSROOT=:ext:visual@access.cims.nyu.edu:/home/visual/cvsroot'
	alias cvsclass='export CVSROOT=:ext:yapclass@access.cims.nyu.edu:/home/yap/cvsroot2'
	alias cvsfriend='export CVSROOT=:ext:e-friend@access.cims.nyu.edu:/home/exact/cvsroot'
	alias cvsfriend2='export CVSROOT=:ext:e-friend@access.cims.nyu.edu:/home/exact/cvsroot2'
	alias cvsyu='export CVSROOT=:ext:gvis@sage.mc.yu.edu:/home/gvis/cvsroot'

alias dummy=  # to prevent folding error by gvim!
#################################################################
# SSH and SFTP  (use -Y for display and gvim across the internet)
#################################################################
	alias myftp='sftp yap@access.cims.nyu.edu'
	alias myssh='ssh -Y yap@access.cims.nyu.edu'
	alias sshexact='ssh -Y exact@access.cims.nyu.edu'
	alias sshvisual='ssh -Y visual@access.cims.nyu.edu'
	alias sshdgs='ssh -Y dgs@access.cims.nyu.edu'
	alias sshclass='ssh -Y yapclass@access.cims.nyu.edu'
	alias sshfriend='ssh -Y e-friend@access.cims.nyu.edu'
	# Jan'12: Drew suggested using linax1 when my remote latex did not work via access:
	alias sshlinax='ssh -Y yap@linax1.cims.nyu.edu'

alias dummy=  # to prevent folding error by gvim!
#################################################################
# TEX
#################################################################
	# on solaris, TEXINPUTS WOULD HAVE BEEN SET UP ALREADY!
	export TEXINPUTS='.:../mac:mac:/Users/cheeyap/mac/cyg_texmf:'${TEXINPUTS}
	export BIBINPUTS='.:ref:../ref:/Users/cheeyap/ref/ref:'${BIBINPUTS}

alias dummy=  # to prevent folding error by gvim!
#################################################################
# SHELL PROMPTS
#################################################################
	# set a default prompt of: user@host current_directory
	# Following does not work?
	# export PS1='\[\033]0;\w\007\033[32m\]\u@\h \[\033[33m\w\033[0m\] $ '
	# 	See ~/prog/shell for more info (Chee)
	# SOME CODES:
	#  \w   = current working directry
	#  \W   = only the LAST directory of current working directry
	#		(GOOD CHOICE since I already put the CWD in title bar)
	#  \h   = host
	#  \u   = user name
	#  \nnn = ASCII code
	#  \[ ... \] = grouping
	# COLORS cods:
	#    Attribute codes:
	#      00=none 01=bold 04=underscore 05=blink 07=reverse 08=concealed
	#    Text color codes:
	#      30=black 31=red 32=green 33=yellow 34=blue 35=magenta 36=cyan 37=white
	#    Background color codes:
	#      40=black 41=red 42=green 43=yellow 44=blue 45=magenta 46=cyan 47=white
	#	(use 0 to turn off background color)
	# CHOICES:
	#(a) Simple (no color prompt): e.g., "~/prob [101> "
	# export PS1simple='\w [\!> '
	#
	#(b) Green/Yellow prompt: e.g., "yap@jinai ~/let[101] "
	# export PS1='\[\033]0;\w\007\033[32m\]\u@\h \[\033[33m\w\033[0m\][\!] '
	#
	#(c) Experimental: e.g., "yap@~/prob[101] "
	#	where 	"yap" is in blue with white background (\033[47;34m\])
	#		"~/prob" in white with black background (\033[40;37m\])
	#		"[101]" in red with no background (\033[0;31m\]) 
	# 	This version puts "XYZ:\w" in title bar, and uses \W in prompt.
	# export PS1exp='\[\033]0;XYZ:\w\007\033[47;34m\]\u @\[\033[40;37m\W\033[0;31m\][\!] '
	#
	#(d) Better for cygwin (dark bg): e.g., "yap@~/prob[101] "
	#	where 	"yap" is in green (\033[32m\])
	#		"~/prob" in yellow (\033[33m\])
	#		"[101]" in black (\033[0m\])
	# 	This version puts \w in title bar, and uses \W in prompt:
	# export PS1cyg='\[\033]0;\w\007\033[32m\]\u@\[\033[33m\W\033[0m\][\!] '
	#
	#(e) Better for solaris (light bg): e.g., "yap@~/prob[101] "
	#	where 	"yap" is in blue with white background (\033[47;34m\])
	#		"~/prob" in red with previous background (\033[31m\])
	#		"[101]" in black with no background (\033[0;30m\]) 
	# 	This version puts "YAP:\w" in title bar, and uses \W in prompt:
	# export PS1sol='\[\033]0;YAP: \w\007\033[47;34m\]\u@\[\033[31m\W \033[0;30m\][\!] '
	#
	#(f) Something for BOTH cygwin and solaris: e.g., "yap@~/prob[101] "
	#	where 	"yap" is in blue with white background (\033[47;34m\])
	#		"~/prob" in red with previous background (\033[31m\])
	#		"[101]" in black with no background (\033[0;30m\]) 
	# 	This version puts "YAP:\w" in title bar, and uses \W in prompt:
	# export PS1='\[\033]0;YAP: \w\007\033[47;34m\]\u@\[\033[31m\W \033[47;36m\][\!] '
	#(g) Customize:
	if [ $_YAP_CYGWIN ]
	then
	   export PS1='\[\033]0;cyg:\w\007\033[47;34m\]\u@\[\033[42;30m\W\033[0m\][\!] '
	else
	   export PS1='\[\033]0;sun:\w\007\033[47;34m\]\u@\[\033[43;31m\W\033[0m\][\!] '
	fi

alias dummy=  # to prevent folding error by gvim!
#################################################################
# Some example functions
#################################################################
alias dummy=  # to prevent folding error by gvim!
	# how to use settitle function below?
	#	Just typing "settitle XXX" seems to replace $@ by "XXX"
	#	but on my MacOS (xterm and std term, it does nothing...)
	function settitle() { echo -n "^[]2;$@^G^[]1;$@^G"; }
	# From ~/prog/xfig/Notes:
	function bashfig () { command xfig -pwidth 9 -pheight 6 
		-lat -spec -startfo 20 -start -fg blue
		-geom 900x650-200+20
		         -but_per_row 3 -startgridmode 1 -startposnmode 1 $* & }
	# The following is used in ~/bin/myfig:
	#
	#   xfig -P -lat -spec -startfo 20 -startg 1 -but_ 3 \
	#	-fg blue -geom 900x650-200+20  $*
	#
	# Options:
	#	-P : ? portrait ?
	#	-lat[exfonts] : start with latex fonts
	#		(instead of default postscript fonts)
	# 	-but_[per_row] N : how many buttons per row (want 3 or 4, not 2)
	# 	-fg blue: color for labels and icons
	#	-spec[ialtext] : start in special mode (for latex processing)
	#	-startfo[ntsize] 20: starting font size
	#	-startg[ridmode] 1: starting size for grid (0=no grid)
	#	-ph[eight] 6:    starting height of panel

alias dummy=  # to prevent folding error by gvim!
#################################################################
# END .bashrc (yap)
#################################################################


#################################################################
#########################Kai Cao#################################
#################################################################
alias ~='home/KC1492'
alias puhedron='cd ~/桌面/corelib2/trunk/progs/gwb/polyhedron'
alias chrome='chromium-browser &'
alias gv='gvim-orig -f'
alias gvim='gvim-orig -f'
alias qt='qtcreator &'
alias puprogs='cd ~/桌面/corelib2/trunk/progs'
alias putrunk='cd ~/桌面/corelib2/trunk'
alias pumcore='pushd ~/study/2012Fall/MCore/'
alias pustudy='pushd ~/study/2012Fall/IndeStudy/'
alias pudoc='pushd ~/桌面/corelib2/trunk/progs/gwb/polyhedron/documents/'
alias pufinal='pushd ~/study/2012Fall/MCore/final/'

export PATH=$PATH:~/Documents/core-2.1.1/gmp/lib/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$PATH:/usr/bin

export PATH=$PATH:~/Documents/core-2.1.1/mpfr/lib/
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$PATH:/usr/bin

export PATH=$PATH:/usr/local/sbin/
export PATH=$PATH:/usr/sbin/
export PATH=$PATH:/sbin/


pudoc

