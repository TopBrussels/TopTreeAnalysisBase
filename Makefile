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
ROOTLIBS       += -L$(ROOFITSYS)/lib

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
SOURCESTOPFCNC   = $(wildcard Content/src/AnalysisEnvironment.cc Content/src/Dataset.cc KinFitter/src/*.cc MCInformation/src/JetPartonMatching.cc MCInformation/src/LumiReWeighting.cc MCInformation/src/ResolutionFit.cc Reconstruction/src/*JetCorr*.cc Reconstruction/src/MEzCalculator.cc Reconstruction/src/Observables.cc Selection/src/Selection*.cc Tools/src/AnalysisEnvironmentLoader.cc Tools/src/JetTools.cc Tools/src/MultiSamplePlot.cc Tools/src/PlottingTools.cc Tools/src/TTreeLoader.cc ../TopTreeAnalysis/TopFCNC/src/TopFCNC_*.cc tinyxml/*.cc)
HEADERSTOPFCNC   = $(wildcard Content/interface/AnalysisEnvironment.h Content/interface/Dataset.h KinFitter/interface/*.h MCInformation/interface/JetPartonMatching.h MCInformation/interface/LumiReWeighting.h MCInformation/interface/ResolutionFit.h  Reconstruction/interface/*JetCorr*.h Reconstruction/interface/MEzCalculator.h Reconstruction/interface/Observables.h Selection/interface/Selection*.h Tools/interface/AnalysisEnvironmentLoader.h Tools/interface/JetTools.h Tools/interface/MultiSamplePlot.h Tools/interface/PlottingTools.h Tools/interface/TTreeLoader.h ../TopTreeAnalysis/TopFCNC/interface/TopFCNC_*.h tinyxml/*.h)
OBJECTSTOPFCNC	 = $(SOURCESTOPFCNC:.$(SrcSuf)=.$(ObjSuf))

SOURCESTOPFCNCDIC	= $(wildcard ../TopTreeAnalysis/TopFCNC/src/TopFCNC_Evt.cc Content/src/Dataset.cc ../TopTreeProducer/src/TRoot*.cc)
HEADERSTOPFCNCDIC	= $(wildcard ../TopTreeAnalysis/TopFCNC/interface/TopFCNC_Evt.h Content/interface/Dataset.h ../TopTreeProducer/interface/TRoot*.h)
OBJECTSTOPFCNCDIC	= $(SOURCESTOPFCNCDIC:.$(SrcSuf)=.$(ObjSuf))

all:  libTopTreeAna76.$(DllSuf) libTopTreeAnaContent76.$(DllSuf)
	cp libTopTreeAna76.$(DllSuf) ~/lib/ ; cp libTopTreeAnaContent76.$(DllSuf) ~/lib/ ; cp Dict_rdict.pcm ~/lib/

btag: libBtagAnalysis76.$(DllSuf)
	cp libBtagAnalysis76.$(DllSuf) ~/lib/

mtop: libMtopAnalysis76.$(DllSuf)
	cp libMtopAnalysis76.$(DllSuf) ~/lib/

topfcnc: libTopFcncAnalysis76.$(DllSuf)
	cp libTopFcncAnalysis76.$(DllSuf) ~/lib/

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(OBJECTSDIC) $(OBJECTSBTAG) $(OBJECTBTAGDIC) $(OBJECTSMTOP) $(OBJECTSMTOPDIC) $(OBJECTSTOPFCNC) $(OBJECTSTOPFCNCDIC) $(DEPENDS) macros/*.exe *Dict.* *.$(DllSuf) core 

.SUFFIXES: .$(SrcSuf) .C .o .$(DllSuf)

###

Dict.$(SrcSuf): $(HEADERSDIC) ./LinkDef.h
	@echo "Generating dictionary Dict..."
	@rootcint -f Dict.$(SrcSuf) -c $(DEFINES) $(HEADERSDIC) ./LinkDef.h

libTopTreeAna76.$(DllSuf): $(OBJECTS) libTopTreeAnaContent76.$(DllSuf)
	@echo "Building libTopTreeAna76..."
	$(LD) $(LIBS) -lTopTreeAnaContent76 $(SOFLAGS) $(LDFLAGS) $+ -o $@

libTopTreeAnaContent76.$(DllSuf): $(OBJECTSDIC)  Dict.o  
	@echo "Building libTopTreeAnaContent76..."
	$(LD) $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

# specific stuff for btag eff analysis ONLY

BtagDict.$(SrcSuf): $(HEADERSBTAGDIC) ./BtagLinkDef.h
	@echo "Generating dictionary BtagDict..."
	@rootcint -f BtagDict.$(SrcSuf) -c $(DEFINES) $(HEADERSBTAGDIC) ./BtagLinkDef.h

libBtagAnalysis76.$(DllSuf): $(OBJECTSBTAG) BtagDict.o
	@echo "Building libBtagAnalysis..."
	$(LD) $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

# specific stuff for mtop analysis ONLY

MtopDict.$(SrcSuf): $(HEADERSMTOPDIC) ./MtopLinkDef.h
	@echo "Generating dictionary MtopDict..."
	@rootcint -f MtopDict.$(SrcSuf) -c $(DEFINES) $(HEADERSMTOPDIC) ./MtopLinkDef.h

libMtopAnalysis76.$(DllSuf): $(OBJECTSMTOP) MtopDict.o
	@echo "Building libMtopAnalysis..."
	$(LD) $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

# specific stuff for top fcnc analysis ONLY

TopFcncDict.$(SrcSuf): $(HEADERSTOPFCNCDIC) ./TopFcncLinkDef.h
	@echo "Generating dictionary TopFcncDict..."
	@rootcint -f TopFcncDict.$(SrcSuf) -c $(DEFINES) $(HEADERSTOPFCNCDIC) ./TopFcncLinkDef.h

libTopFcncAnalysis76.$(DllSuf): $(OBJECTSTOPFCNC) TopFcncDict.o
	@echo "Building libTopFcncAnalysis..."
	$(LD) $(LIBS_NoTMVA) $(SOFLAGS) $(LDFLAGS) $+ -o $@

ADDLIBS_MACROS = -lMLP -lTreePlayer -lXMLIO

macros/%.exe: macros/%.cc $(HEADERS) libTopTreeAna76.$(DllSuf) libTopTreeAnaContent76.$(DllSuf)
	$(LD) -lTopTreeAna76 -lTopTreeAnaContent76 $(LIBS) $(ADDLIBS_MACROS) -I`root-config --incdir` $< $(LDFLAGS) -o $@

SOURCES_MACROS = $(wildcard macros/*.cc)

macros: $(SOURCES_MACROS:.cc=.exe)

