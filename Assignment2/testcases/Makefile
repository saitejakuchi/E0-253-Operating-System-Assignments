CC := gcc
CFLAGS := -O3
LDFLAGS := 
ALL := compare

all: $(ALL)

target: fork.c context.c compare.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
clean:
	rm -f $(ALL)
