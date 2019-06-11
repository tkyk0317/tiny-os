.PHONY: all clean start stop
all:
	@ cd build; make -s

clean:
	@ cd build; make -s clean

start:
	@ cd build; qemu-system-aarch64 -m 128 -monitor telnet::1234,server,nowait -M raspi3 -kernel tiny-os -serial mon:stdio

stop:
	@ kill `pgrep qemu`

objdump:
	@ aarch64-linux-gnu-objdump -S --prefix-addresses build/tiny-os | less
