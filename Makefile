all:
	gcc -std=c11 -O3 main.c iam.c Shape.c Rectangle.c Triangle.c -o iam_test
