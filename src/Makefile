$(info Introduction to MPI)

SHELL ?= /usr/bin/bash # Define shell for the build scripts

ROOT_DIR := $(realpath $(shell pwd))

MPI ?= off # Enable MPI on executables that use MPI conditionally
BUILD_DIR ?= $(strip $(ROOT_DIR)/build) # Define the outpur directory

include make/build_setup.mk

CC := gcc
MPICC := mpicc
CFLAGS ?= -O2 -Wall
LDFLAGS ?= $(CFLAGS)

AR := ar
ARFLAGS := rcs

INSTALL_HEADER := install --mode=u=rw,g=r,o=r -Dt

MPICC_FLEX = $(CC)
CFLAGS_FLEX = $(CFLAGS)

ifeq ($(MPI), on)
MPICC_FLEX = $(MPICC)
CFLAGS_FLEX += -D_MPI
_MPI := on
else
_MPI := off
endif

CACHE := _MPI

$(foreach var, $(CACHE), $(eval export $(var)))

include make/build_cache.mk

HELP_OUTPUT_CMD := $(shell \
  if echo "test" | column --table-columns-limit=2 1>/dev/null 2>&1; then \
    echo "column --table --table-columns-limit=2"; \
  else \
    echo "cat"; \
  fi\
)

.PHONY: help
help: # Shows interactive help.
	@bash -c '([ -f INSTALL.md ] && cat INSTALL.md || true)'
	@echo
	@echo "make variables:"
	@sed -e 's/^\([^\ \t]\+\)[\ \t]*?=[^#]\+#\+\(.*\)$$/\1 \2/p;d' Makefile \
        | $(HELP_OUTPUT_CMD) \
        | sort
	@echo
	@echo "make targets:"
	@echo $(TARGETS) | tr ' ' '\n' | xargs -I % bash -c 'echo "-  %"'
	@echo
	@echo "make special targets:"
	@sed -e 's/^\([^:\ \t]\+\):.*#\+\(.*\)$$/\1 \2/p;d' Makefile \
        | $(HELP_OUTPUT_CMD) \
        | sort

include utils/build.mk
include simple_examples/build.mk
include exercises/build.mk

.PHONY: clean
clean: # Clear contents of build directory
	@echo "Removing files:"
	@if [ -d $(BUILD_DIR) ]; then \
	    find $(BUILD_DIR) -type f -print0 | xargs -0I % bash -c '{ echo "%"; rm "%"; }'; \
	    rm -r $(BUILD_DIR); \
	fi
