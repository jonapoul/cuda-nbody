CXX      = g++ -std=c++17
CXXFLAGS = -c -Wall
LD       = $(CXX)
LDFLAGS  = -Wall

ROOTDIR := .
OBJDIR  := obj
BINDIR  := bin
SRCDIR  := src
INCDIR  := inc

LIBS :=
#-lfolly -lglog -lgflags -ldouble-conversion -lboost_regex

INC  = -I. \
       -I$(INCDIR)

SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

EXEC = percolation

default: dir $(EXEC)

$(EXEC): $(OBJ) | dir
	$(LD) -o $(BINDIR)/$@ $(LDFLAGS) $(OBJ) $(LIBS)

dir:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

clean:
	rm -f $(OBJDIR)/*   2>/dev/null || true
	rm -f $(BINDIR)/*   2>/dev/null || true
	rm -f $(SRCDIR)/*.o 2>/dev/null || true

diff:
	git diff --stat

rebuild: clean $(EXEC)

update:
	git pull

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | dir
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@