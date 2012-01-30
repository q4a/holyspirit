@echo off
title Holyspirit debug session
gdb.exe --command=gdbcom>fulldebuglog.txt
dosmsg "Holyspirit_debug.exe has encountered an error. Some debugging informations have been saved in the file 'fulldebuglog.txt'. Please contact Holyspirit's support team at http://www.game-corp.net/ or at http://holyspirit.alpha-arts.net/." "Holyspirit Debug" 64