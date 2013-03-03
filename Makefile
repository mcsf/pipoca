# $Id: Makefile,v 1.3 2010-05-12 17:42:16 ist162462 Exp $
# $Log: Makefile,v $
# Revision 1.3  2010-05-12 17:42:16  ist162462
# Makefile: alternative path for ROOT
#
# Revision 1.2  2010-05-11 16:03:12  ist162510
# Makefile root conflict solved
#
# Revision 1.1  2010/05/09 21:04:47  ist162510
# Added makefile.
#
# Revision 1.9  2010/03/22 15:34:20  david
# Changed code to be compatible with CDK5. For Pipoca, this is only
# a matter of changing a compilation flag. For other compilers, it opens
# a number of sophisticated possibilities.
#
# Revision 1.8  2009/03/03 17:45:22  david
# Minor cleanup.
#
# Revision 1.7  2009/03/02 17:40:20  david
# Major changes: in addition to compiling with CDK4, Pipoca now has
# its own namespace (pipoca). All classes are defined in it or in its
# child namespaces. Added automatic node processing: the "nodes"
# directory is scanned and nodes/all.h is built (contains both forward
# declarations and include directives for all nodes -- in the appropriate
# namespaces).
#
#---------------------------------------------------------------
#             CONFIGURE THESE VARIABLES IF NEEDED
#---------------------------------------------------------------
ROOT = ${HOME}/compiladores/root
ifeq ($(wildcard $(ROOT)),) 
    ROOT = ${HOME}/ist/com/root
endif

CDK_INC_DIR = $(ROOT)/usr/include
CDK_LIB_DIR = $(ROOT)/usr/lib

L_NAME=PipocaScanner
Y_NAME=PipocaParser

#---------------------------------------------------------------
#    PROBABLY, NO NEED TO CHANGE ANYTHING BEYOND THIS POINT
#---------------------------------------------------------------

LFLAGS   = 
YFLAGS   = -dtv
# CXXFLAGS = -Wall -O6 -s -Isemantics -I. -I$(CDK_INC_DIR) -I$(CDK_INC_DIR)/cdk
CXXFLAGS = -DYYDEBUG=1 -Wall -ggdb -Isemantics -I. -I$(CDK_INC_DIR) -I$(CDK_INC_DIR)/cdk
LDFLAGS  = -L$(CDK_LIB_DIR) -lcdk
COMPILER = pipoca

SRC_CPP = $(wildcard nodes/**/*.cpp) $(wildcard semantics/*.cpp) $(wildcard ./*.cpp)
OFILES  = $(SRC_CPP:%.cpp=%.o)

#---------------------------------------------------------------
#                DO NOT CHANGE AFTER THIS LINE
#---------------------------------------------------------------

all: nodes/all.h $(COMPILER)

%.tab.o:: %.tab.c
	$(CXX) $(CXXFLAGS) -Wno-write-strings -c $< -o $@

%.o:: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o:: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.tab.c:: %.y
	 byacc -dtv -b $* $<

%.tab.h:: %.y
	 byacc -dtv -b $* $<

$(L_NAME).cpp: $(L_NAME).l
	flex $(LFLAGS) $<

$(Y_NAME).tab.c: $(Y_NAME).y
$(Y_NAME).tab.h: $(Y_NAME).y

# this is needed to force byacc to run
$(L_NAME).o: $(L_NAME).cpp $(Y_NAME).tab.h

.PHONY: ./mknodedecls.pl
nodes/all.h: ./mknodedecls.pl
	./mknodedecls.pl > nodes/all.h

$(COMPILER): $(L_NAME).o $(Y_NAME).tab.o $(OFILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) *.tab.[ch] *.o $(OFILES) $(L_NAME).cpp $(Y_NAME).output $(COMPILER)

depend: nodes/all.h
	$(CXX) $(CXXFLAGS) -MM $(SRC_CPP) > .makedeps

-include .makedeps

#---------------------------------------------------------------
#                           THE END
#---------------------------------------------------------------
