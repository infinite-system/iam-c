all:
	gcc -std=c11 -O2 main.c iam.c Shape.c Rectangle.c Triangle.c AreaByHeightSide.c -o iam_test
