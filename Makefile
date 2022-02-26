all:
	gcc -o mai -fsanitize=address ./main.c
