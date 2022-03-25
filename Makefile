CC := gcc
CFLAGS := -O1 -g -Wall -Werror

TEST_DIR := test
OBJS := u128.o ubign.o $(TEST_DIR)/fibonacci.o
DEPS := $(OBJS:%.o=.%.o.d)

test: fibonacci

# Control the build verbosity
ifeq ($(VERBOSE), 1)
    Q :=
    VECHO := @true
else
    Q := @
    VECHO := @printf
endif


fibonacci: $(OBJS)
	$(VECHO) "LD\t$@\n"
	$(Q) $(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	-@mkdir -p .$(TEST_DIR)
	$(VECHO) "CC\t$@\n"
	$(Q) $(CC) $(CFLAGS) -o $@ -c -MMD -MF .$@.d $<

.PHONY: clean
clean:
	rm -rf $(OBJS) $(DEPS) fibonacci

-include $(DEPS)