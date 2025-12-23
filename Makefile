all:
	gcc -std=c11 -O2 \
		main.c \
		module/iam/core/IAM.c \
		module/iam/example/ancestry/IAM_GrandParent.c \
		Shape.c \
		Rectangle.c \
		Triangle.c \
		AreaByHeightSide.c \
		-o iam_test