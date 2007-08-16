EXTRA_DIST = NEWS README AUTHORS INSTALL ChangeLog COPYING
SUBDIRS = include src 
AUTOMAKE_OPTIONS = foreign

backup: clean
	tar cvzf ../backup/`date +%F_%H_%S_%M`".tar" *
