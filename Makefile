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

# headers and sources for mtop analysis lib
SOURCESMTOPDIC	= $(wildcard JESMeasurement/src/LightMonster.cc Content/src/Dataset.cc)
HEADERSMTOPDIC	= $(wildcard JESMeasurement/interface/LightMonster.h Content/interface/Dataset.h)
OBJECTSMTOPDIC	= $(SOURCESMTOPDIC:.$(SrcSuf)=.$(ObjSuf))

SOURCESMTOP   = $(wildcard JESMeasurement/src/MonsterTools.cc JESMeasurement/src/LightMonster.cc Tools/src/MultiSamplePlot.cc Content/src/Dataset.cc Tools/src/PlottingTools.cc MCInformation/src/*ReWeighting.cc MCInformation/src/ResolutionFit.cc ../TopQuarkAnalysis/TopMassIdeogram/src/*.cc JESMeasurement/IdeogramTools/src/*.cc)
HEADERSMTOP   = $(wildcard JESMeasurement/interface/MonsterTools.h JESMeasurement/interface/LightMonster.h Tools/interface/MultiSamplePlot.h Content/interface/Dataset.h Tools/interface/PlottingTools.h MCInformation/interface/*ReWeighting.h MCInformation/interface/ResolutionFit.h ../TopQuarkAnalysis/TopMassIdeogram/interface/*.h JESMeasurement/IdeogramTools/interface/*.h)
OBJECTSMTOP		= $(SOURCESMTOP:.$(SrcSuf)=.$(ObjSuf))

# headers and sources for anomcoup analysis lib
SOURCESANOMCOUPDIC =$(wildcard ../AnomalousCouplings/PersonalClasses/src/*.cc) 
HEADERSANOMCOUPDIC = $(wildcard ../AnomalousCouplings/PersonalClasses/interface/*.h)
OBJECTSANOMCOUPDIC = $(SOURCESANOMCOUPDIC:.$(SrcSuf)=.$(ObjSuf))

SOURCESANOMCOUP = $(wildcard ../AnomalousCouplings/PersonalClasses/src/*.cc)
HEADERSANOMCOUP = $(wildcard ../AnomalousCouplings/PersonalClasses/interface/*.h)
OBJECTSANOMCOUP = $(SOURCESANOMCOUP:.$(SrcSuf)=.$(ObjSuf))

all:  libTopTreeAna53.$(DllSuf) libTopTreeAnaContent53.$(DllSuf)
	cp libTopTreeAna53.$(DllSuf) ~/lib/ ; cp libTopTreeAnaContent53.$(DllSuf) ~/lib/

mtop: libMtopAnalysis53.$(DllSuf)
	cp libMtopAnalysis53.$(DllSuf) ~/lib/

anomcoup: libAnomCoupAnalysis53.$(DllSuf)
	mv libAnomCoupAnalysis53.$(DllSuf) ~/lib/

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(OBJECTSDIC) $(OBJECTSMTOP) $(OBJECTSMTOPDIC) $(OBJECTSANOMCOUP) $(OBJECTSANOMCOUPDIC) $(DEPENDS) macros/*.exe *Dict.* *.$(DllSuf) core 

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

# specific stuff for mtop analysis ONLY

MtopDict.$(SrcSuf): $(HEADERSMTOPDIC) ./MtopLinkDef.h
	@echo "Generating dictionary MtopDict..."
	@rootcint -f MtopDict.$(SrcSuf) -c $(DEFINES) $(HEADERSMTOPDIC) ./MtopLinkDef.h

libMtopAnalysis53.$(DllSuf): $(OBJECTSMTOP) MtopDict.o
	@echo "Building libMtopAnalysis..."
	$(LD) $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

# headers and sources for anomcoup analysis lib
AnomCoupDict.$(SrcSuf): $(HEADERSANOMCOUPDIC) ./AnomCoupLinkDef.h
	@echo "Generating dictionary AnomCoupDict..."
	@rootcint -f AnomCoupDict.$(SrcSuf) -c $(DEFINES) $(HEADERSANOMCOUPDIC) ./AnomCoupLinkDef.h

libAnomCoupAnalysis53.$(DllSuf): $(OBJECTSANOMCOUP) AnomCoupDict.o
	@echo "Building libAnomCoupAnalysis..."
	$(LD) $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

ADDLIBS_MACROS = -lMLP -lTreePlayer -lXMLIO

macros/%.exe: macros/%.cc $(HEADERS) libTopTreeAna53.$(DllSuf) libTopTreeAnaContent53.$(DllSuf)
	$(LD) -lTopTreeAna53 -lTopTreeAnaContent53 $(LIBS) $(ADDLIBS_MACROS) -I`root-config --incdir` $< $(LDFLAGS) -o $@

SOURCES_MACROS = $(wildcard macros/*.cc)

macros: $(SOURCES_MACROS:.cc=.exe)

