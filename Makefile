.PHONY: all clean start stop
all:
	@ mkdir -p build; cd build; cmake ..; make -s

clean:
	@ cd build; make -s clean
	@ rm -rf tiny-os

start: all
	@ cd build; qemu-system-aarch64 -cpu cortex-a53 -m 1g -nographic -monitor telnet::1234,server,nowait -M raspi3 -kernel tiny-os -serial mon:stdio

stop:
	@ kill `pgrep qemu`

objdump:
	@ aarch64-linux-gnu-objdump -S --prefix-addresses build/tiny-os | less

telnet:
	@ telnet localhost 1234

gdb-server:
	@ qemu-system-aarch64 -m 1g -cpu cortex-a53  -nographic -monitor telnet::1234,server,nowait -M raspi3 -kernel build/tiny-os -serial mon:stdio -gdb tcp::4321 -S

gdb-client:
	@ aarch64-linux-gnu-gdb -i=mi build/tiny-os
