DAT = C:/MinGW/allegsvn/4.2/tools/dat.exe

DATAFILE = defnot.dat
HEADFILE = ../include/ids.h

all:
# 	-rm $(DATAFILE)
	$(DAT) -bpp 8 -t BMP -a $(DATAFILE) bitmaps/*.pcx
	$(DAT) -t PAL -a $(DATAFILE) bitmaps/palette.pcx
	$(DAT) -t FONT -a $(DATAFILE) fonts/font.pcx
# 	$(DAT) -t SAMP -a $(DATAFILE) data/*.wav
# 	$(DAT) -t MIDI -a $(DATAFILE) data/*.mid
	$(DAT) -h $(HEADFILE) -c1 $(DATAFILE)
