SILENT  = @
PREFIX  = autobc
TSEQ    = $(PREFIX)-seq
TPAR    = $(PREFIX)-par
TCSE    = $(PREFIX)-cse
TARGETS = $(TSEQ) $(TPAR) $(TCSE)

OBJDIR  = obj
SRCSSEQ = autobc-seq.cpp common.cpp
SRCSPAR = autobc-par.cpp common.cpp
SRCSCSE = $(filter-out $(wildcard autobc-*.cpp), $(wildcard *.cpp)) autobc-cse.cpp
OBJSSEQ = $(SRCSSEQ:%.cpp=$(OBJDIR)/%.o)
OBJSPAR = $(SRCSPAR:%.cpp=$(OBJDIR)/%.o)
OBJSCSE = $(SRCSCSE:%.cpp=$(OBJDIR)/%.o)
OBJS    = $(OBJSSEQ) $(OBJSPAR)
CVFLAGS = $(shell pkg-config opencv4 --cflags)
CVLIBS  = $(shell pkg-config opencv4 --libs)
CPPFLAGS= -pthread
CXXFLAGS= -g -std=c++11 $(CVFLAGS)
LDFLAGS = -lpthread $(CVLIBS)

all: $(TARGETS)


$(TPAR): $(OBJSPAR)
	$(SILENT) $(CXX) $(CPPFLAGS) -o $@ $? $(LDFLAGS)


$(TSEQ): $(OBJSSEQ)
	$(SILENT) $(CXX) $(CPPFLAGS) -o $@ $? $(LDFLAGS)


$(TCSE): $(OBJSCSE)
	$(SILENT) $(CXX) $(CPPFLAGS) -o $@ $? $(LDFLAGS)


$(OBJDIR)/%.o: %.cpp
	$(SILENT) mkdir -p $(dir $@)
	$(SILENT) $(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -MMD -o $@


.PHONY: run run-seq run-par run-cse
run: $(TARGETS)
# 	$(SILENT) ./$(TSEQ)
# 	$(SILENT) ./$(TPAR)
	$(SILENT) ./$(TCSE)

run-seq: $(TSEQ)
	$(SILENT) ./$(TSEQ)

run-par: $(TPAR)
	$(SILENT) ./$(TPAR)

run-cse: $(TCSE)
	$(SILENT) ./$(TCSE)


.PHONY: clean
clean:
	$(SILENT) rm -f $(TARGETS)
	$(SILENT) rm -rf $(OBJDIR)/*

-include $(OBJDIR)/*.d
