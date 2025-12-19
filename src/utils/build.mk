LIBS := array multiply

LIB_ARRAY := array
LIB_MULTIPLY := multiply

.PHONY: libraries
libraries: $(LIBS)

.PHONY: $(LIBS)
$(LIBS): %: $(LIB_DIR)/lib%.a

LIB_ARRAY_HEADERS_PUBLIC := $(INCLUDE_DIR)/array.h
LIB_MULTIPLY_HEADERS_PUBLIC := $(INCLUDE_DIR)/multiply.h

LIB_ARRAY_LINK_OPTIONS_PUBLIC :=
LIB_MULTIPLY_LINK_OPTIONS_PUBLIC := -lm

UTIL_SRC_DIR:=$(patsubst %/, %, $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))
UTIL_OBJ_DIR:=$(strip $(OBJ_DIR))/util

LIB_ARRAY_HEADERS := $(patsubst $(INCLUDE_DIR)/%, $(UTIL_SRC_DIR)/%, $(LIB_ARRAY_HEADERS_PUBLIC))
LIB_MULTIPLY_HEADERS := $(patsubst $(INCLUDE_DIR)/%, $(UTIL_SRC_DIR)/%, $(LIB_MULTIPLY_HEADERS_PUBLIC))

$(patsubst %, $(UTIL_OBJ_DIR)/%.o, $(LIB_ARRAY)): $(UTIL_OBJ_DIR)/%.o: $(UTIL_SRC_DIR)/%.c $(LIB_ARRAY_HEADERS) | $(UTIL_OBJ_DIR)
	$(CC) -c $(CFLAGS) -I$(UTIL_SRC_DIR) $< -o $@

$(patsubst %, $(UTIL_OBJ_DIR)/%.o, $(LIB_MULTIPLY)): $(UTIL_OBJ_DIR)/%.o: $(UTIL_SRC_DIR)/%.c $(LIB_MULTIPLY_HEADERS) $(LIB_ARRAY_HEADERS) | $(UTIL_OBJ_DIR)
	$(CC) -c $(CFLAGS) -I$(UTIL_SRC_DIR) $< -o $@

$(LIB_DIR)/libarray.a: $(patsubst %, $(UTIL_OBJ_DIR)/%.o, $(LIB_ARRAY)) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_ARRAY_HEADERS_PUBLIC): $(INCLUDE_DIR)/%: $(UTIL_SRC_DIR)/%
	$(INSTALL_HEADER) $(INCLUDE_DIR) $<

$(LIB_DIR)/libmultiply.a: $(patsubst %, $(UTIL_OBJ_DIR)/%.o, $(LIB_MULTIPLY)) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_MULTIPLY_HEADERS_PUBLIC): $(INCLUDE_DIR)/%: $(UTIL_SRC_DIR)/%
	$(INSTALL_HEADER) $(INCLUDE_DIR) $<
