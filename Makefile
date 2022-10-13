CXX = g++
CFLAGS_COMMON = -std=c++20 -Wall -Wextra
CFLAGS = -O3 $(CFLAGS_COMMON)
CFLAGS_UNOPTIMIZED = -O0 $(CFLAGS_COMMON)
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

