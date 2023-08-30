# 定义共享库文件和头文件的路径
LIBRARY_SRC = dio_driver.c
LIBRARY_SO = libdio_driver.so
HEADER = dio_driver.h

CC = gcc
CFLAGS = -Wall -fPIC

# 定义安装目录
INSTALL_DIR = /usr/local

all: libdio_driver.so

libdio_driver.so: dio_driver.c
	$(CC) $(CFLAGS) -shared -o $@ $^


install: $(LIBRARY_SO)
	install -m 644 $(HEADER) $(INSTALL_DIR)/include
	install -m 755 $(LIBRARY_SO) $(INSTALL_DIR)/lib

clean:
	rm -f $(LIBRARY_SO)

uninstall:
	rm -f $(INSTALL_DIR)/include/$(HEADER)
	rm -f $(INSTALL_DIR)/lib/$(LIBRARY_SO)
