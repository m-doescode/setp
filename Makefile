CXX = clang++

LIBS = 
INCLUDES = include
MACROS = UNICODE
CXXFLAGS = -static-libgcc -static-libstdc++ -std=c++2b -Wall -Wextra -fexceptions -fuse-ld=lld $(addprefix -l,$(LIBS)) $(addprefix -I,$(INCLUDES)) $(addprefix -D,$(MACROS))
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

clean:
	@rm -r $(OBJDIR)/**

$(BUILDDIR)/$(EXENAME): $(OBJS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@