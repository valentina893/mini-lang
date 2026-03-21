cc = gcc
target = mini

src_dir = src
obj_dir = bin

src = $(wildcard $(src_dir)/*.c)
obj = $(patsubst $(src_dir)/%.c,$(obj_dir)/%.o,$(src))

cflags = -Wall -Wextra -std=c11 -O2

$(obj_dir)/%.o: $(src_dir)/%.c
	mkdir -p $(obj_dir)
	$(cc) $(cflags) -c $< -o $@

$(target): $(obj)
	$(cc) $(obj) -o $@ $(sdl_libs)

clean:
	rm -rf $(obj_dir) $(target)

.PHONY: clean