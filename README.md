# ficses_uio
Userspace I/O binding for FiCSW by nyacom (C) 2019.11

----

## What is this?
ficses_uio is a binding FiCSES device to UIO (Userspace IO) which is a part of Linux Kernel.

----

## Install
Just simply like this:

    % make clean && make install

The command builds the ficses_uio module and install and load to your kernel.

You can check the module if loaded: 

    nyacom@ficses:~$ lsmod|grep ficses_uio
    ficses_uio             16384  0
    uio                    20480  1 ficses_uio

## Uninstall
Just simply liks this:

    % make uninstall
    
The command unloads ficses_uio from kernel and remove the module from your kernel directory.

----

## How to use?

If ficses_uio loaded successfuly, the UIO maps FiCSES in /dev/uioX so you can mmap the device.

In more detail, you can check under /sys/class/uio/uioX/maps/mapX/, there are usually like:

    nyacom@ficses:~$ ls /sys/class/uio/uio0/maps/map0/
    addr  name  offset  size

- _addr_ is physical address of corresponding this map.
- _name_ is a name of this memory secion. (Always NULL In ficses_uio)
- _offset_ is a offset of the map secion. (Always 0x0 in ficses_uio)
- _size_ is a size of this map. (4194304 for BA0, 4096 for BA1)

In python, you can use pyficses_uio.py for userspace driver. Maybe you can understand how to use.
