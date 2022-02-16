TARGET = a.out
OBJS = main.o tar.o
SRCS = $(OBJS:%.o=%.c)
CC = gcc
CFLAGS =
LDFLAGS =

$(TARGET):$(OBJS)
		$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

.PHONY: clean

clean:
	rm $(TARGET) $(OBJS)