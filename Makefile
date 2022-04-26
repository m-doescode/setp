CXX = clang++

LIBS = 
INCLUDES = include
CXXFLAGS = -g -std=c++2b -Wall -Wextra -fexceptions -fuse-ld=lld $(addprefix -l,$(LIBS)) $(addprefix -I,$(INCLUDES))
LDFLAGS = -lm

SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)

OBJDIR = obj
OBJS = $(SRCS:$(SRCDIR)%.cpp=$(OBJDIR)%.o)

BUILDDIR = build
EXENAME = setp.exe

all: $(BUILDDIR)/$(EXENAME)

test: all
	$(BUILDDIR)/$(EXENAME)

$(BUILDDIR)/$(EXENAME): $(OBJS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@