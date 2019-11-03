KDIR := /lib/modules/$(shell uname -r)/build
KMOD := /lib/modules/$(shell uname -r)/
TARGET := ficses_uio.ko
obj-m += ficses_uio.o

.PHONY : all clean install

all : $(TARGET)

$(TARGET): $(TARGET:.ko=.c)
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install: $(TARGET)
	cp $(TARGET) $(KMOD)
	depmod -a
	modprobe uio
	modprobe ficses_uio

uninstall:
	modprobe -r ficses_uio
	rm -rf $(KMOD)/$(TARGET)
