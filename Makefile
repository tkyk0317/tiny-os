.PHONY: all clean start stop
all:
	@ cd build; make -s

clean:
	@ cd build; make -s clean

start:
	@ cd build; qemu-system-aarch64 -m 128 -nographic -M raspi3 -kernel tiny-os -serial mon:stdio

stop:
	@ kill `pgrep qemu`
