SHADERS = $(wildcard shaders/*.vert) $(wildcard shaders/*.frag)
SHADER_OUTPUT_DIR = ../../build/shaders

$(SHADER_OUTPUT_DIR):
	mkdir -p $(SHADER_OUTPUT_DIR)

$(SHADER_OUTPUT_DIR)/%.spv: shaders/%
	glslc -c $< -o $@

.PHONY: shaders
shaders: | $(SHADER_OUTPUT_DIR)
	$(foreach shader,$(SHADERS),$(MAKE) $(SHADER_OUTPUT_DIR)/$(notdir $(shader)).spv;)
