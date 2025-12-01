$(info Introduction to MPI)

CC ?= mpicc
CFLAGS ?= -O2 -Wall
LDFLAGS ?= $(CFLAGS)

TARGETS_BARE=1.1.MPI_hello_world 1.2.MPI_hello_world_PP 2.1.MPI_bcast
TARGETS_WITH_UTILS=2.2.MPI_bcast_arrays 2.3.MPI_bcast_matrices_error 2.4.MPI_bcast_matrices_vs1 2.5.MPI_bcast_matrices_vs2 3.1.MPI_send_recv_arrays 4.1.MPI_scatter_gather_arrays 5.1.MPI_reduce_allreduce

BUILD_DIR?=build ## Define the outpur directory

BIN_DIR=$(strip $(BUILD_DIR))/bin
BIN_BARE_DIR=$(strip $(BIN_DIR))/bare
BIN_WITH_UTILS_DIR=$(strip $(BIN_DIR))/with_utils

OBJ_DIR=$(strip $(BUILD_DIR))/obj
OBJ_BARE_DIR=$(strip $(OBJ_DIR))/bare
OBJ_WITH_UTILS_DIR=$(strip $(OBJ_DIR))/with_utils
OBJ_LIB_DIR=$(strip $(OBJ_DIR))/lib

SRC_DIR=simple_examples
LIB_SRC=util

LIB_OBJS=$(patsubst $(SRC_DIR)/$(LIB_SRC)/%.c, $(OBJ_LIB_DIR)/%.o, $(wildcard $(SRC_DIR)/$(LIB_SRC)/*.c))
LIB_HEADERS=$(wildcard $(SRC_DIR)/$(LIB_SRC)/*.h)

.PHONY: help
help: # Shows interactive help.
	@cat README.md
	@echo
	@echo "make variables:"
	@echo
	@sed -e 's/^\([^\ \t]\+\)[\ \t]*?=[^#]\+#\+\(.*\)$$/\1 \2/p;d' Makefile \
        | column --table --table-columns-limit=2 \
        | sort
	@echo
	@echo "make targets:"
	@echo
	@echo Targets without utilities: $(TARGETS_BARE)
	@echo Targets with utilities: $(TARGETS_WITH_UTILS)
	@echo
	@echo "make special targets:"
	@echo
	@sed -e 's/^\([^:\ \t]\+\):.*#\+\(.*\)$$/\1 \2/p;d' Makefile \
        | column --table --table-columns-limit=2 \
        | sort

$(BUILD_DIR):
	mkdir --parents "$(BUILD_DIR)"

$(BIN_DIR): | $(BUILD_DIR)
	mkdir "$(BIN_DIR)"

$(BIN_BARE_DIR): | $(BIN_DIR)
	mkdir "$(BIN_BARE_DIR)"

$(BIN_WITH_UTILS_DIR): | $(BIN_DIR)
	mkdir "$(BIN_WITH_UTILS_DIR)"

$(OBJ_DIR): | $(BIN_DIR)
	mkdir "$(OBJ_DIR)"

$(OBJ_BARE_DIR) : | $(OBJ_DIR)
	mkdir "$(OBJ_BARE_DIR)"

$(OBJ_WITH_UTILS_DIR) : | $(OBJ_DIR)
	mkdir "$(OBJ_WITH_UTILS_DIR)"

$(OBJ_LIB_DIR) : | $(OBJ_DIR)
	mkdir "$(OBJ_LIB_DIR)"

$(OBJ_LIB_DIR)/%.o: $(SRC_DIR)/$(LIB_SRC)/%.c $(LIB_HEADERS) | $(OBJ_LIB_DIR)
	$(CC) -c $(CFLAGS) -I$(SRC_DIR)/$(LIB_SRC) $@

.PHONY: TARGETS_BARE
$(TARGETS_BARE): %: $(BIN_BARE_DIR)/%

$(BIN_BARE_DIR)/%: $(OBJ_BARE_DIR)/%.o | $(BIN_BARE_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_BARE_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_BARE_DIR)
	$(CC) -c $(CFLAGS) $< $@

.PHONY: TARGETS_WITH_UTILS
$(TARGETS_WITH_UTILS): %: $(BIN_WITH_UTILS_DIR)/%

$(BIN_WITH_UTILS_DIR)/%: $(OBJ_WITH_UTILS_DIR)/%.o $(LIB_OBJS) | $(BIN_WITH_UTILS_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_WITH_UTILS_DIR)/%.o: $(SRC_DIR)/%.c $(LIB_HEADERS) | $(OBJ_WITH_UTILS_DIR)
	$(CC) -c $(CFLAGS) -I$(SRC_DIR)/$(LIB_SRC) $< $@

.PHONY: clean
clean:
	@echo "Removing files:"
	@if [ -d $(BUILD_DIR) ]; then \
	    find $(BUILD_DIR) -type f -print0 | xargs -0I % bash -c '{ echo "%"; rm "%"; }'; \
	    rm -r $(BUILD_DIR); \
	fi
