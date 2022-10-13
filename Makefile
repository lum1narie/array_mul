CXX = g++
CFLAGS = -O3 -std=c++20
CFLAGS_UNOPTIMIZED = -O0 -std=c++20
TARGET = main
TARGETS = $(BUILDDIR)/$(TARGET) $(BUILDDIR)/$(TARGET)_unoptimized
SRCS = main.cpp
BUILDDIR = build

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/

$(BUILDDIR)/$(TARGET): $(SRCS) $(BUILDDIR)
	$(CXX) $(CFLAGS) -o $@ $<

$(BUILDDIR)/$(TARGET)_unoptimized: $(SRCS) $(BUILDDIR)
	$(CXX) $(CFLAGS_UNOPTIMIZED) -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

