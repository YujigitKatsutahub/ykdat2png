##### edited by yuji katsuta 2023.05.12
PROG=ykdat2png

OBJS = $(PROG).o 
#OBJS = $(PROG).o 

#HDRS = 

ifndef RELEASE
CFLAGS = -DLINUX -Wall -ggdb
else
CFLAGS = -DLINUX -Wall -O2 -pipe
endif

All : $(PROG)

$(PROG) : $(OBJS)
	$(CC)  -o $(PROG)  $(OBJS) $(LDFLAGS) $(LDF)

$(PROG).o: $(PROG).c $(HDRS)
	$(CC) $(CFLAGS) -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(PROG) $(OBJS)

pre:
	$(CC) $(CFLAGS) -E $(PROG).c -o $(PROG).i
	$(CC) $(CFLAGS) -S $(PROG).i -o $(PROG).s

##### end of file
