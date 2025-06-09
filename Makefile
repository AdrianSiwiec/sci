#Project structure
INCDIR=inc
SRCDIR=src
OBJDIR=obj
TESTDIR=test

#G++ compiler settings
CXX=g++
CXXINC=-I ./$(INCDIR)/
# CXXFLAGS= -std=c++17 $(CXXINC) -g -rdynamic -Werror=return-type -Werror -Wall -Wno-sign-compare 
# CXXFLAGS=-O3 -std=c++17 $(CXXINC) -g -rdynamic -Werror=return-type -Wall -Wno-sign-compare 
CXXFLAGS=-Og -std=c++17 $(CXXINC) -g -rdynamic -Werror=return-type -Werror -Wall -Wno-sign-compare -pg -g

INCFILES=$(wildcard $(INCDIR)/*.h)
SRCFILES=$(wildcard $(SRCDIR)/*.cpp)
TESTFILES=$(wildcard $(TESTDIR)/*.cpp)
OBJFILES=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCFILES))
UNITTESTFILES=$(patsubst %.cpp, $(OBJDIR)/%.e, $(wildcard $(TESTDIR)/*.cpp))
RUNTIMEFILES=$(patsubst %.cpp, $(OBJDIR)/%.e, $(wildcard $(SRCDIR)/*.cpp))

# $(info $(UNITTESTFILES))

#Targets
unitTest: $(UNITTESTFILES)
main: $(UNITTESTFILES)
infiTest: $(UNITTESTFILES)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.e: %.cpp $(OBJFILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJFILES) $< -o $@

$(OBJDIR)/%.html: %.cpp $(OBJFILES)
	@mkdir -p $(dir $@)
	$(EMCC) $(CXXFLAGS) -c $< -o $@

.PHONY: test clean run-tests unitTest retest netTest lint site

# Do not delete OBJFILES by default (shortens make time)
.OBJ: $(OBJFILES)

unitTest: 
	./test/runUnitTest.sh

clean:
	rm -rf obj *.e
	rm -rf site/out

main:
	./obj/test/main.e

infiTest:
	./obj/test/infiTest.e

site:
	./site/build_site.sh
