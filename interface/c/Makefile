CC=/usr/bin/gcc
LDFLAGS=-lcrypto

SOURCES= cofhe_main.c uart_access_api.c
SOURCES2= cofhe_file.c uart_access_api.c cofhe_tasks.c
HEADERS= uart_access.h

NO_UART: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(LDFLAGS)


UART_ON: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(LDFLAGS) -DUART_ON

UART_FILE: $(HEADERS) $(SOURCES2)
	$(CC) -o $@ $(SOURCES2) $(LDFLAGS) -DUART_FILE -lm

.PHONY: clean

clean:
	-rm *UART*

