CC := gcc
LD := gcc
TARGET := stree

SRC := ./src
INC := ./inc
OBJ := ./obj

OBJECTS := $(OBJ)/util.o\
           $(OBJ)/sexpr.o\
           $(OBJ)/tree.o\
           $(OBJ)/stree.o\

DEPS := $(OBJECTS:.o=.d)

CFLAGS  := -I$(INC)
LDFLAGS := -lc

.PHONY: all
all: $(TARGET)

debug: CFLAGS += -g
debug: $(TARGET)

test: CFLAGS += -g
test: $(TARGET)-test

ifneq ($(filter clean,$(MAKECMDGOALS)),clean)
-include $(DEPS)
endif

$(TARGET): $(OBJECTS)
	@echo [LD] -o $@ $^
	@$(LD) $(LDFLAGS) -o $@ $^

$(TARGET)-test: CFLAGS += -DTEST
$(TARGET)-test: $(OBJECTS) $(OBJ)/test.o
	@echo [LD] -o $@ $^
	@$(LD) $(LDFLAGS) --entry test -o $@ $^

$(DEPS) $(OBJECTS): | $(OBJ)/

$(OBJ)/%.o: $(SRC)/%.c
	@echo [CC] -o $@ $<
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.d: $(SRC)/%.c
	@echo [DEP] $<
	@set -e; rm -f $@; \
	$(CC) -MM -MF $@ $(CFLAGS) $<;\
	sed -i 's,\($*\)\.o[\s:]*,\1.o $@: ,g' $@;

%/:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r $(OBJ) $(TARGET) $(TARGET)-test

