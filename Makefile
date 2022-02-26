all:
	gcc -o mai -fsanitize=address ./h.c
