CXX = g++
CFLAGS = -O3 -std=c++20
TARGET = main
SRCS = main.cpp
BUILDDIR = build

$(BUILDDIR)/$(TARGET): $(SRCS) $(BUILDDIR)
	$(CXX) $(CFLAGS) -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)
