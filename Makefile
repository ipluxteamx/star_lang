exec = star
src = $(wildcard src/*.c)
obj = $(src:.c=.o)
flags = -g

$(exec): $(obj)
	gcc $(obj) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	cp ./$(exec) /usr/local/bin/$(exec)

clean:
	rm -f $(exec) $(obj)