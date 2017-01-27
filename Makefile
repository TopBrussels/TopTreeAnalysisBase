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
ROOTLIBS        = $(shell root-config --libs)
ROOTLIBS_NoTMVA = $(shell root-config --libs)
ROOTGLIBS       = $(shell root-config --glibs)
ifeq ($(UNAME), Linux)
ROOTLIBS       += -lMinuit -lMathMore -lMinuit2 -lRooFitCore -lRooFit -lRooStats -lFoam -lTMVA
ROOTLIBS_NoTMVA+= -lMinuit -lMathMore -lMinuit2 -lRooFitCore -lRooFit -lRooStats -lFoam
ROOTGLIBS      += -lMinuit -lMathMore -lMinuit2 -lRooFitCore -lRooFit -lRooStats -lFoam -lTMVA
endif
ROOTLIBS       += -L$(ROOFITSYS)/lib
ROOTLIBS_NoTMVA+= -L$(ROOFITSYS)/lib

# Linux with egcs
DEFINES         = -DNO_ORCA_CLASSES -I..
CXX             = g++
CXXFLAGS	= -O -Wall -fPIC $(DEFINES)  -I./TMVA/include
ifeq ($(UNAME), Darwin)
CXXFLAGS       += -I/opt/local/include
endif
LD		= g++
LDFLAGS		= -g -O -Wall -fPIC
ifeq ($(UNAME), Darwin)
SOFLAGS         = -dynamiclib
endif
ifeq ($(UNAME), Linux)
SOFLAGS         = -shared
endif

CXXFLAGS       += $(ROOTCFLAGS)
LIBS            = -I./TMVA/include -L./TMVA/lib $(ROOTLIBS) -lEG -I.. -L. -L../TopTreeProducer/src
LIBS_NoTMVA     = $(ROOTLIBS_NoTMVA) -lEG -I.. -L. -L../TopTreeProducer/src
ifeq ($(UNAME), Darwin)
LIBS           += -I/opt/local/include
endif
GLIBS		= $(ROOTGLIBS)
#------------------------------------------------------------------------------
SOURCES         = $(wildcard Tools/src/*.cc Selection/src/*.cc Reconstruction/src/*.cc MCInformation/src/*.cc tinyxml/*.cc KinFitter/src/*.cc)
HEADERS         = $(wildcard Tools/interface/*.h Selection/interface/*.h Reconstruction/interface/*.h MCInformation/interface/*.h tinyxml/*.h Kinfitter/interface/*.h)
OBJECTS		= $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
DEPENDS		= $(SOURCES:.$(SrcSuf)=.d)
SOBJECTS	= $(SOURCES:.$(SrcSuf)=.$(DllSuf))
#for libTopTreeAnaContent.so
SOURCESDIC	= $(wildcard Reconstruction/src/Observables.cc Reconstruction/src/MEzCalculator.cc Content/src/*.cc ../TopTreeProducer/src/TRoot*.cc)
HEADERSDIC	= $(wildcard Content/interface/*.h ../TopTreeProducer/interface/TRoot*.h)
OBJECTSDIC	= $(SOURCESDIC:.$(SrcSuf)=.$(ObjSuf))

all:  libTopTreeAna76.$(DllSuf) libTopTreeAnaContent76.$(DllSuf)
	cp libTopTreeAna76.$(DllSuf) ~/lib/ ; cp libTopTreeAnaContent76.$(DllSuf) ~/lib/ ; cp Dict_rdict.pcm ~/lib/

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(OBJECTSDIC) $(DEPENDS) macros/*.exe *Dict.* *.$(DllSuf) core 

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

ADDLIBS_MACROS = -lMLP -lTreePlayer -lXMLIO

macros/%.exe: macros/%.cc $(HEADERS) libTopTreeAna76.$(DllSuf) libTopTreeAnaContent76.$(DllSuf)
	$(LD) -lTopTreeAna76 -lTopTreeAnaContent76 $(LIBS) $(ADDLIBS_MACROS) -I`root-config --incdir` $< $(LDFLAGS) -o $@

SOURCES_MACROS = $(wildcard macros/*.cc)

macros: $(SOURCES_MACROS:.cc=.exe)

