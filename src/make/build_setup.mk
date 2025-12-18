MAKE_SCRIPT_DIR:=$(patsubst %/, %, $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

BIN_DIR=$(strip $(BUILD_DIR))/bin
OBJ_DIR=$(strip $(BUILD_DIR))/obj
LIB_DIR=$(strip $(BUILD_DIR))/lib
INCLUDE_DIR=$(strip $(BUILD_DIR))/include
CACHE_DIR=$(strip $(BUILD_DIR))/.cache

$(BUILD_DIR):
	mkdir --parents "$(strip $(BUILD_DIR))"

$(BIN_DIR): | $(BUILD_DIR)
	mkdir "$(BIN_DIR)"

$(OBJ_DIR): | $(BUILD_DIR)
	mkdir "$(OBJ_DIR)"

$(OBJ_DIR)/%: | $(OBJ_DIR)
	mkdir --parents "$@"

$(CACHE_DIR): | $(BUILD_DIR)
	mkdir $(CACHE_DIR)

$(LIB_DIR): | $(BUILD_DIR)
	mkdir $(LIB_DIR)

$(INCLUDE_DIR): | $(BUILD_DIR)
	mkdir $(INCLUDE_DIR)
