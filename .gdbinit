#set detach-on-fork off
#set follow-fork-mode child
set disassemble-next-line on
show disassemble-next-line
handle SIGILL nostop pass
break crash_break
source gdb.py
