CFLAGS = -std=c11 -O2 -Isrc -DIAMC_DEBUG

SRCS = \
  src/module/iam/core/Kernel.c \
  src/module/iam/example/ancestry/GrandParent.c \
  src/app/main.c \
  src/app/Shape.c \
  src/app/Rectangle.c \
  src/app/Triangle.c \
  src/app/AreaByHeightSide.c

all: iam_test

iam_test: $(SRCS)
	gcc $(CFLAGS) $(SRCS) -o iam_test

clean:
	rm -f iam_test
.PHONY: all clean