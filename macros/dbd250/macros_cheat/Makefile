ROOTFLAG = `root-config --cflags`
ROOTLIB = `root-config --libs`
ROOTLIB += -lTreePlayer
ROOTLIB += -lMLP -lTMVA -lMinuit -lXMLIO -lRooFit

all:fit

fit:fit.o fit_recoil.o
	g++ $(ROOTLIB) $^ -o $@

fit.o:fit.cpp
	g++ -c $(ROOTFLAG) $< -o $@

fit_recoil.o:fit_recoil.C readData.C readCut.C global.h readPara.C
	g++ -c $(ROOTFLAG) $< -o $@
