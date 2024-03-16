#Project structure
INCDIR=inc
SRCDIR=src
OBJDIR=obj
TESTDIR=test

#G++ compiler settings
CXX=g++
CXXINC=-I ./$(INCDIR)/
CXXFLAGS=-O3 -std=c++11 $(CXXINC) -g -rdynamic -Werror=return-type

INCFILES=$(wildcard $(INCDIR)/*.h)
SRCFILES=$(wildcard $(SRCDIR)/*.cpp)
TESTFILES=$(wildcard $(TESTDIR)/*.cpp)
OBJFILES=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCFILES))
UNITTESTFILES=$(patsubst %.cpp, $(OBJDIR)/%.e, $(wildcard $(TESTDIR)/*.cpp))
RUNTIMEFILES=$(patsubst %.cpp, $(OBJDIR)/%.e, $(wildcard $(SRCDIR)/*.cpp))

# $(info $(OBJFILES))
# $(info $(TESTFILES))

#Targets
unitTest: $(UNITTESTFILES)
main: $(UNITTESTFILES)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.e: %.cpp $(OBJFILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJFILES) $< -o $@ 

.PHONY: test clean run-tests unitTest retest netTest lint

# Do not delete OBJFILES by default (shortens make time)
.OBJ: $(OBJFILES)

unitTest: 
	./test/runUnitTest.sh

clean:
	rm -rf obj *.e

main:
	./obj/test/main.e