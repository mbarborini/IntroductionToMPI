$(foreach var, $(CACHE), $(eval $(var)_CACHE=$(strip $(CACHE_DIR))/$(var)))

.PHONY: $(patsubst %, UPDATE_%_CACHE, $(CACHE))
$(patsubst %, UPDATE_%_CACHE, $(CACHE)): UPDATE_%_CACHE: | $(CACHE_DIR)
	$(eval VAR_NAME = $(patsubst UPDATE_%_CACHE, %, $@))
	$(eval VAR_VALUE = $(value $(strip $(patsubst UPDATE_%_CACHE, %, $@))))
	$(eval CACHE_FILE = $(strip $(CACHE_DIR))/$(strip $(patsubst UPDATE_%_CACHE, %, $@)))
	if [ -f "$(strip $(CACHE_FILE))" ]; then \
	    OLD_$(VAR_NAME)="$$(cat "$(CACHE_FILE)")"; \
	else \
	    OLD_$(strip $(VAR_NAME))=""; \
	fi; \
	if [ "$${OLD_$(VAR_NAME)}" != "$(VAR_VALUE)" ]; then \
	    echo "$(VAR_VALUE)" > "$(CACHE_FILE)"; \
	fi

$(patsubst %, $(CACHE_DIR)/%, $(CACHE)): $(CACHE_DIR)/%: UPDATE_%_CACHE
