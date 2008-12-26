EXTRA_DIST = NEWS README AUTHORS INSTALL ChangeLog COPYING
SUBDIRS = include src 
AUTOMAKE_OPTIONS = foreign


backup: dist 
	mkdir -p ../backup
	mv *.tar.gz ../backup/`date +%F_%H_%M_%S`".tar" 

restore: backup
	dialog --fselect ../backup/ 15 60 2>respuesta
	tar xvzf `cat respuesta`
	rm -f respuesta

print:
	enscript -T4 -G2rE Test.cpp -pdata.ps
	ps2pdf data.ps
	rm -f data.ps
