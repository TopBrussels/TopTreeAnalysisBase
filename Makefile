ObjSuf        = o
SrcSuf        = cc
ExeSuf        = 
UNAME = $(shell uname -s)
ifeq ($(UNAME), Darwin)
DllSuf        = dylib
endif
ifeq ($(UNAME), Linux)
DllSuf        = so
endif

OutPutOpt     = -o
HeadSuf       = h

ROOTCFLAGS      = $(shell root-config --cflags)
ROOTLIBS        = $(shell root-config --libs) -lMinuit -lMathMore -lMinuit2 -lRooFitCore -lRooFit -lRooStats -lFoam -lTMVA
ROOTLIBS_NoTMVA = $(shell root-config --libs) -lMinuit -lMathMore -lMinuit2 -lRooFitCore -lRooFit -lRooStats -lFoam
ROOTGLIBS       = $(shell root-config --glibs) -lMinuit -lMathMore -lMinuit2 -lRooFitCore -lRooFit -lRooStats -lFoam -lTMVA

# Linux with egcs
DEFINES       = -DNO_ORCA_CLASSES -I..
CXX           = g++
CXXFLAGS	= -O -Wall -fPIC $(DEFINES)  -I./TMVA/include
ifeq ($(UNAME), Darwin)
CXXFLAGS        += -I/opt/local/include
endif
LD		= g++
LDFLAGS		= -g -O -Wall -fPIC
ifeq ($(UNAME), Darwin)
SOFLAGS         = -dynamiclib
endif
ifeq ($(UNAME), Linux)
SOFLAGS         = -shared
endif

CXXFLAGS	     += $(ROOTCFLAGS)
LIBS		        = -I./TMVA/include -L./TMVA/lib $(ROOTLIBS) -lEG -I.. -L. -L../TopTreeProducer/src 
LIBS_NoTMVA     = $(ROOTLIBS_NoTMVA) -lEG -I.. -L. -L../TopTreeProducer/src 
ifeq ($(UNAME), Darwin)
LIBS           += -I/opt/local/include
endif
GLIBS		= $(ROOTGLIBS)
#------------------------------------------------------------------------------
SOURCES         = $(wildcard Tools/src/*.cc StatProcedure/src/*.cc BkgEstimationMethods/src/*.cc  Selection/src/*.cc Reconstruction/src/*.cc MCInformation/src/*.cc tinyxml/*.cc KinFitter/src/*.cc JESMeasurement/src/*.cc WHelicities/src/*.cc InclFourthGenSearch/src/*.cc)
HEADERS         = $(wildcard Tools/interface/*.h StatProcedure/interface/*.h BkgEstimationMethods/interface/*.h  Selection/interface/*.h Reconstruction/interface/*.h MCInformation/interface/*.h tinyxml/*.h Kinfitter/interface/*.h JESMeasurement/interface/*.h WHelicities/interface/*.h InclFourthGenSearch/interface/*.h)
OBJECTS		= $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
DEPENDS		= $(SOURCES:.$(SrcSuf)=.d)
SOBJECTS	= $(SOURCES:.$(SrcSuf)=.$(DllSuf))
#for libTopTreeAnaContent.so
SOURCESDIC	= $(wildcard Reconstruction/src/Observables.cc Reconstruction/src/MEzCalculator.cc Content/src/*.cc ../TopTreeProducer/src/TRoot*.cc JESMeasurement/src/Monster.cc JESMeasurement/src/LightMonster.cc WHelicities/src/WTree.cc InclFourthGenSearch/src/InclFourthGenTree.cc BkgEstimationMethods/src/VJetEstimation.cc)
HEADERSDIC	= $(wildcard Content/interface/*.h ../TopTreeProducer/interface/TRoot*.h JESMeasurement/interface/Monster.h JESMeasurement/interface/LightMonster.h WHelicities/interface/WTree.h InclFourthGenSearch/interface/InclFourthGenTree.h BkgEstimationMethods/interface/VJetEstimation.h)
OBJECTSDIC	= $(SOURCESDIC:.$(SrcSuf)=.$(ObjSuf))

# headers and sources for btag eff analysis lib
SOURCESBTAGDIC	= $(wildcard BtagEffAnalysis/src/TRoot*.cc Tools/src/MultiSamplePlot.cc Content/src/Dataset.cc)
HEADERSBTAGDIC	= $(wildcard BtagEffAnalysis/interface/TRoot*.h Tools/interface/MultiSamplePlot.h Content/interface/Dataset.h)
OBJECTSBTAGDIC	= $(SOURCESDIC:.$(SrcSuf)=.$(ObjSuf))

SOURCESBTAG         = $(wildcard BtagEffAnalysis/src/*.cc Tools/src/MultiSamplePlot.cc Content/src/Dataset.cc Tools/src/PlottingTools.cc)
HEADERSBTAG         = $(wildcard BtagEffAnalysis/interface/*.h Tools/interface/MultiSamplePlot.h Content/interface/Dataset.h Tools/interface/PlottingTools.h)
OBJECTSBTAG		= $(SOURCESBTAG:.$(SrcSuf)=.$(ObjSuf))

# headers and sources for mtop analysis lib
SOURCESMTOPDIC	= $(wildcard JESMeasurement/src/LightMonster.cc Content/src/Dataset.cc)
HEADERSMTOPDIC	= $(wildcard JESMeasurement/interface/LightMonster.h Content/interface/Dataset.h)
OBJECTSMTOPDIC	= $(SOURCESMTOPDIC:.$(SrcSuf)=.$(ObjSuf))

SOURCESMTOP   = $(wildcard JESMeasurement/src/MonsterTools.cc JESMeasurement/src/LightMonster.cc Tools/src/MultiSamplePlot.cc Content/src/Dataset.cc Tools/src/PlottingTools.cc MCInformation/src/*ReWeighting.cc MCInformation/src/ResolutionFit.cc ../TopQuarkAnalysis/TopMassIdeogram/src/*.cc JESMeasurement/IdeogramTools/src/*.cc)
HEADERSMTOP   = $(wildcard JESMeasurement/interface/MonsterTools.h JESMeasurement/interface/LightMonster.h Tools/interface/MultiSamplePlot.h Content/interface/Dataset.h Tools/interface/PlottingTools.h MCInformation/interface/*ReWeighting.h MCInformation/interface/ResolutionFit.h ../TopQuarkAnalysis/TopMassIdeogram/interface/*.h JESMeasurement/IdeogramTools/interface/*.h)
OBJECTSMTOP		= $(SOURCESMTOP:.$(SrcSuf)=.$(ObjSuf))

# headers and sources for top fcnc analysis lib
SOURCESTOPFCNC   = $(wildcard Content/src/AnalysisEnvironment.cc Content/src/Dataset.cc KinFitter/src/*.cc MCInformation/src/JetPartonMatching.cc MCInformation/src/LumiReWeighting.cc MCInformation/src/ResolutionFit.cc Reconstruction/src/*JetCorr*.cc Reconstruction/src/MEzCalculator.cc Reconstruction/src/Observables.cc Selection/src/Selection*.cc Tools/src/AnalysisEnvironmentLoader.cc Tools/src/JetTools.cc Tools/src/MultiSamplePlot.cc Tools/src/PlottingTools.cc Tools/src/TTreeLoader.cc TopFCNC/src/TopFCNC_*.cc tinyxml/*.cc)
HEADERSTOPFCNC   = $(wildcard Content/interface/AnalysisEnvironment.h Content/interface/Dataset.h KinFitter/interface/*.h MCInformation/interface/JetPartonMatching.h MCInformation/interface/LumiReWeighting.h MCInformation/interface/ResolutionFit.h  Reconstruction/interface/*JetCorr*.h Reconstruction/interface/MEzCalculator.h Reconstruction/interface/Observables.h Selection/interface/Selection*.h Tools/interface/AnalysisEnvironmentLoader.h Tools/interface/JetTools.h Tools/interface/MultiSamplePlot.h Tools/interface/PlottingTools.h Tools/interface/TTreeLoader.h TopFCNC/interface/TopFCNC_*.h tinyxml/*.h)
OBJECTSTOPFCNC	 = $(SOURCESTOPFCNC:.$(SrcSuf)=.$(ObjSuf))

SOURCESTOPFCNCDIC	= $(wildcard TopFCNC/src/TopFCNC_Evt.cc Content/src/Dataset.cc ../TopTreeProducer/src/TRoot*.cc)
HEADERSTOPFCNCDIC	= $(wildcard TopFCNC/interface/TopFCNC_Evt.h Content/interface/Dataset.h ../TopTreeProducer/interface/TRoot*.h)
OBJECTSTOPFCNCDIC	= $(SOURCESTOPFCNCDIC:.$(SrcSuf)=.$(ObjSuf))

all:  libTopTreeAna53.$(DllSuf) libTopTreeAnaContent53.$(DllSuf)
	cp libTopTreeAna53.$(DllSuf) ~/lib/ ; cp libTopTreeAnaContent53.$(DllSuf) ~/lib/

btag: libBtagAnalysis53.$(DllSuf)
	cp libBtagAnalysis53.$(DllSuf) ~/lib/

mtop: libMtopAnalysis53.$(DllSuf)
	cp libMtopAnalysis53.$(DllSuf) ~/lib/

topfcnc: libTopFcncAnalysis53.$(DllSuf)
	cp libTopFcncAnalysis53.$(DllSuf) ~/lib/

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(OBJECTSDIC) $(OBJECTSBTAG) $(OBJECTBTAGDIC) $(OBJECTSMTOP) $(OBJECTSMTOPDIC) $(OBJECTSTOPFCNC) $(OBJECTSTOPFCNCDIC) $(DEPENDS) macros/*.exe *Dict.* *.$(DllSuf) core 

.SUFFIXES: .$(SrcSuf) .C .o .$(DllSuf)

###

Dict.$(SrcSuf): $(HEADERSDIC) ./LinkDef.h
	@echo "Generating dictionary Dict..."
	@rootcint -f Dict.$(SrcSuf) -c $(DEFINES) $(HEADERSDIC) ./LinkDef.h

libTopTreeAna53.$(DllSuf): $(OBJECTS) libTopTreeAnaContent53.$(DllSuf)
	@echo "Building libTopTreeAna53..."
	$(LD) $(LIBS) -lTopTreeAnaContent53 $(SOFLAGS) $(LDFLAGS) $+ -o $@

libTopTreeAnaContent53.$(DllSuf): $(OBJECTSDIC)  Dict.o  
	@echo "Building libTopTreeAnaContent53..."
	$(LD) $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

# specific stuff for btag eff analysis ONLY

BtagDict.$(SrcSuf): $(HEADERSBTAGDIC) ./BtagLinkDef.h
	@echo "Generating dictionary BtagDict..."
	@rootcint -f BtagDict.$(SrcSuf) -c $(DEFINES) $(HEADERSBTAGDIC) ./BtagLinkDef.h

libBtagAnalysis53.$(DllSuf): $(OBJECTSBTAG) BtagDict.o
	@echo "Building libBtagAnalysis..."
	$(LD) $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

# specific stuff for mtop analysis ONLY

MtopDict.$(SrcSuf): $(HEADERSMTOPDIC) ./MtopLinkDef.h
	@echo "Generating dictionary MtopDict..."
	@rootcint -f MtopDict.$(SrcSuf) -c $(DEFINES) $(HEADERSMTOPDIC) ./MtopLinkDef.h

libMtopAnalysis53.$(DllSuf): $(OBJECTSMTOP) MtopDict.o
	@echo "Building libMtopAnalysis..."
	$(LD) $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

# specific stuff for top fcnc analysis ONLY

TopFcncDict.$(SrcSuf): $(HEADERSTOPFCNCDIC) ./TopFcncLinkDef.h
	@echo "Generating dictionary TopFcncDict..."
	@rootcint -f TopFcncDict.$(SrcSuf) -c $(DEFINES) $(HEADERSTOPFCNCDIC) ./TopFcncLinkDef.h

libTopFcncAnalysis53.$(DllSuf): $(OBJECTSTOPFCNC) TopFcncDict.o
	@echo "Building libTopFcncAnalysis..."
	$(LD) $(LIBS_NoTMVA) $(SOFLAGS) $(LDFLAGS) $+ -o $@

ADDLIBS_MACROS = -lMLP -lTreePlayer -lXMLIO

macros/%.exe: macros/%.cc $(HEADERS) libTopTreeAna53.$(DllSuf) libTopTreeAnaContent53.$(DllSuf)
	$(LD) -lTopTreeAna53 -lTopTreeAnaContent53 $(LIBS) $(ADDLIBS_MACROS) -I`root-config --incdir` $< $(LDFLAGS) -o $@

SOURCES_MACROS = $(wildcard macros/*.cc)

macros: $(SOURCES_MACROS:.cc=.exe)

