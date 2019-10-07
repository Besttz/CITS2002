PROJECT		= h

HEADER		= $(PROJECT).h
OBJ			=cal.o degree.o haversine.o vaild.o


# ----------------------------------------------------------------

CC      = cc
CFLAGS  = -std=c99 -Wall -Werror -pedantic

$(PROJECT) : $(OBJ)
	$(CC) $(CFLAGS) -o $(PROJECT) $(OBJ) 

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clear:
	rm -f $(OBJ)

