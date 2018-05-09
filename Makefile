CXX      = g++ -std=c++17
CXXFLAGS = -c $(LDFLAGS)
LD       = $(CXX)

# https://www.reddit.com/r/programming/comments/8gh0cq/gcc_81_released/dybxx5t/
LDFLAGS  = -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option

ROOTDIR := .
OBJDIR  := obj
BINDIR  := bin
SRCDIR  := src
INCDIR  := inc
LOGDIR  := logs

FOLLY_LIBS := -lfolly -lglog -lgflags -ldouble-conversion -lboost_regex
BOOST_LIBS := -lboost_system -lboost_filesystem -lboost_regex
LIBS = $(BOOST_LIBS)

INC  = -I. \
       -I$(INCDIR)

SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

EXEC = cuda_nbody

default: dir $(EXEC)

$(EXEC): $(OBJ) | dir
	$(LD) -o $(BINDIR)/$@ $(LDFLAGS) $(OBJ) $(LIBS)

dir:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LOGDIR)

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