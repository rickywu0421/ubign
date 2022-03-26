CC := gcc
#CFLAGS := -O1 -g -Wall -Werror
CFLAGS := -O1 -g -Wall -fno-stack-protector

TEST_DIR := test
TEST_EXEC := fibonacci
OBJS := u128.o ubign.o $(TEST_DIR)/$(TEST_EXEC).o
DEPS := $(OBJS:%.o=.%.o.d)

test: $(TEST_EXEC)

# Control the build verbosity
ifeq ($(VERBOSE), 1)
    Q :=
    VECHO := @true
else
    Q := @
    VECHO := @printf
endif


$(TEST_EXEC): $(OBJS)
	$(VECHO) "LD\t$@\n"
	$(Q) $(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	-@mkdir -p .$(TEST_DIR)
	$(VECHO) "CC\t$@\n"
	$(Q) $(CC) $(CFLAGS) -o $@ -c -MMD -MF .$@.d $<

.PHONY: clean
clean:
	rm -rf $(OBJS) $(DEPS) $(TEST_EXEC)

-include $(DEPS)