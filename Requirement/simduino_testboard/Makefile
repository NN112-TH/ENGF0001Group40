.ONESHELL:

all:
	export SIMAVR_UART_XTERM=1
	bin/arduino-cli compile -b arduino:avr:uno sketch

clean:
	ps -aux | grep 'picocom' | grep -v grep | awk '{print $$2}' | xargs -r kill -9
	ps -aux | grep './simduino.elf' | grep -v grep | awk '{print $$2}' | xargs -r kill -9
	rm -rf sketch/build
	rm -rf bin/*_flash.bin

install:
	sudo apt update
	sudo apt install -y build-essential libelf-dev avr-libc gcc-avr freeglut3-dev libncurses5-dev pkg-config avrdude picocom libgtk-3-dev arduino curl
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	bin/arduino-cli core update-index
	bin/arduino-cli core install arduino:avr
	chmod +x bin/*

.PHONY: serial upload run run_simduino

run_simduino:
	./simduino.elf

serial: 
	picocom /tmp/simavr-uart0

upload: 
	avrdude -p m328p -c arduino -P /tmp/simavr-uart0 -U flash:w:sketch/build/arduino.avr.uno/sketch.ino.hex
	picocom /tmp/simavr-uart0

run:	
	make run_simduino -C bin -f ../Makefile

