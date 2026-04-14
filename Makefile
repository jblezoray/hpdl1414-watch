
# see https://arduino.github.io/arduino-cli/1.0/
# see https://www.nongnu.org/avrdude/user-manual/avrdude.html
-include config.mk

AVRDUDE_ISP_CONF= -v  -C $(AVRDUDE_CONF)  -p atmega328p  -c arduino -P $(PORT) -b115200
AVRDUDE_TARGET_CONF=-v  -C $(AVRDUDE_CONF)  -p atmega328p  -c stk500v1 -P $(PORT) -b19200

### META

.PHONY: help
## Show this help.
help:
	@awk ' \
				BEGIN   { section=""; help=""; command="" } \
				/^### / { section=substr($$0, 5); printf "\n=== %s \n", section; next } \
				/^## /  { help=substr($$0, 3); next } help \
		            && /^[[:alpha:]][[:alnum:]_-]+:/ { printf "%25s   %s\n", $$1, help } \
								{ help=0 } \
		' $(MAKEFILE_LIST)

.PHONY: print-config
## print Makefile configuration 
print-config:
	@echo "config:"
	@echo "   ARDUINO_CLI =${ARDUINO_CLI}"
	@echo "   AVRDUDE     =${AVRDUDE}"
	@echo "   AVRDUDE_CONF=${AVRDUDE_CONF}"
	@echo ""


### UTILS

.PHONY: list-devices
## list arduino devices plugged in.
list-devices:
	${ARDUINO_CLI} board list

.PHONY: list-available-avr-parts
## List the available avr-parts. 
list-available-avr-parts:
	$(AVRDUDE) -v  -C $(AVRDUDE_CONF)  -p "?"

.PHONY: detect-isp
## Detect and show the ISP config.
detect-isp:
	$(AVRDUDE) $(AVRDUDE_ISP_CONF) -v

.PHONY: detect-target
## Detect and show the target chip config.
detect-target:
	$(AVRDUDE) $(AVRDUDE_TARGET_CONF) -v


.PHONY: monitor
## monitor
monitor:
	@$(ARDUINO_CLI) monitor \
		-p "$(PORT)" \
		--fqbn "$(FQBN)" \
		--timestamp 

.PHONY: monitor-describe
## monitor describe
monitor-describe:
	@$(ARDUINO_CLI) monitor \
		-p "$(PORT)" \
		--fqbn "$(FQBN)" \
		--describe

### ISP

## Clean all built artifacts.
clean:
	rm -Rf build/

./build/ArduinoISP/ArduinoISP.ino:
	@mkdir -p ./build/ArduinoISP
	wget -nv "$(ISP_SOURCE)" -O ./build/ArduinoISP/ArduinoISP.ino

./build/ArduinoISP/build/arduino.avr.uno/ArduinoISP.ino.hex: ./build/ArduinoISP/ArduinoISP.ino
	$(ARDUINO_CLI) core update-index
	$(ARDUINO_CLI) core install arduino:avr
	cd ./build/ArduinoISP && \
		$(ARDUINO_CLI) \
			compile \
			--fqbn "$(FQBN)" \
			--export-binaries ArduinoISP.ino \

.PHONY: dl-isp
dl-isp: ./build/ArduinoISP/ArduinoISP.ino

.PHONY: compile-isp
compile-isp: ./build/ArduinoISP/build/arduino.avr.uno/ArduinoISP.ino.hex

.PHONY: flash-isp-programmer
## Flash the ISP with ArduinoISP.
flash-isp-programmer: ./build/ArduinoISP/build/arduino.avr.uno/ArduinoISP.ino.hex
	$(AVRDUDE) $(AVRDUDE_ISP_CONF) \
		-D "-Uflash:w:./build/ArduinoISP/build/arduino.avr.uno/ArduinoISP.ino.hex:i"


### FUSES

.PHONY: read-target-clock-fuses
## Read the target clock fuses.
read-target-clock-fuses:
	$(AVRDUDE) $(AVRDUDE_TARGET_CONF) \
		-U lfuse:r:-:h \
		-U hfuse:r:-:h \
		-U efuse:r:-:h

# default fuses are : (E:FF, H:D9, L:62)
# L:0x62 is for: internal oscillator at 8Mhz DIV8, so 1MHz
# L:0xE2 is for: internal oscillator at 8Mhz, no division (bit CKDIV8 deactivated)
# L:0x7F is for: external oscillator at 8-16Mhz DIV8, so 1-2MHz
# L:0xFF is for: external oscillator at 8-16Mhz, no division.
# see https://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p and https://www.martyncurrey.com/arduino-atmega-328p-fuse-settings/
.PHONY: set-target-clock-fuses
## Set the Target clock fuses.
set-target-clock-fuses:
	$(AVRDUDE) $(AVRDUDE_TARGET_CONF) \
		-U lfuse:w:0xE2:m \
		-U hfuse:w:0xD9:m \
		-U efuse:w:0xFF:m


### SOFT

.PHONY: compile
## Compile the project. 
compile:
	$(ARDUINO_CLI) compile \
		--clean \
		--fqbn "$(FQBN)" \
		--export-binaries \
		--output-dir "./build/hpdl-1414" \
		"./hpdl-1414" \
		--build-property "build.extra_flags=-DF_CPU=8000000L" \

.PHONY: upload
## Fash the target with the soft.
upload: compile
	$(AVRDUDE) $(AVRDUDE_TARGET_CONF) \
		-U "flash:w:./build/hpdl-1414/hpdl-1414.ino.hex:i"

