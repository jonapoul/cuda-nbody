CXX      = g++ -std=c++17
CXXFLAGS = -c -g $(LDFLAGS)
LD       = $(CXX) $(LDFLAGS)

# https://www.reddit.com/r/programming/comments/8gh0cq/gcc_81_released/dybxx5t/
LDFLAGS  = -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option

ROOTDIR := .
OBJDIR  := obj
BINDIR  := bin
SRCDIR  := src
INCDIR  := inc
LOGDIR  := logs
TRAJDIR := traj
PARAMSDIR := params

FOLLY_LIBS := -lfolly -lglog -lgflags -ldouble-conversion -lboost_regex
BOOST_LIBS := -lboost_system -lboost_filesystem -lboost_regex

PF_DIR  := /home/jon/uni/mphys/src/libPF
PF_LIBS := -L$(PF_DIR) -lPF

LIBS    := $(BOOST_LIBS) $(PF_LIBS)

INC  = -I$(ROOTDIR) \
       -I$(INCDIR) \
       -I$(PF_DIR)

SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

EXEC = cnb

all: $(EXEC)

$(EXEC): $(OBJ) | dir
	$(LD) -o $(BINDIR)/$@ $(OBJ) $(LIBS)

dir:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LOGDIR)
	mkdir -p $(TRAJDIR)
	mkdir -p $(PARAMSDIR)

clean:
	rm -f $(OBJDIR)/*   2>/dev/null || true
	rm -f $(BINDIR)/*   2>/dev/null || true
	rm -f $(SRCDIR)/*.o 2>/dev/null || true

delog:
	rm -f $(LOGDIR)/* 2>/dev/null || true
	rm -f $(TRAJDIR)/* 2>/dev/null || true

diff:
	git diff --stat

rebuild: clean $(EXEC)

update:
	git pull

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | dir
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@