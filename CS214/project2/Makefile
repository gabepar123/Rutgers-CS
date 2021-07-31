all:
	gcc -g -Wvla -Wall -fsanitize=address,undefined -pthread -o compare *.c -lm

clean:
	rm -f $(OBJS) $(OUT)
