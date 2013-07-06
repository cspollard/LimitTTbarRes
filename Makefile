all: runBCMTFAF.cxx
	g++ runBCMTFAF.cxx `root-config --cflags --libs` -lMinuit -I$(BATINSTALLDIR)/include/ -L$(BATINSTALLDIR)/lib/ -lBAT -lBATmtf -o runBCMTFAF
