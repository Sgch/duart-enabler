CC=arm-mingw32ce-gcc

CFLAGS=-mwin32 -mcpu=arm926ej-s \
		-DUNICODE -D_UNICODE -D_WIN32_WCE=0x600 -O0 -static

TARGETS=AppMain.exe index.din

.PHONY: all clean

all: $(TARGETS)

clean:
	rm $(TARGETS)

AppMain.exe: duart_enabler.c
	$(CC) $(CFLAGS) $< -o $@
 
index.din:
	touch $@
