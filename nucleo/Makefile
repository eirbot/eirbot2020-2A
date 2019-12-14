##
# Nucleo
#
# @file
# @version 0.1

all: compile send upload

compile:
	pio run -e real

send:
	scp .pio/build/real/firmware.elf pi@169.254.48.235:

upload:
	ssh pi@169.254.48.235 'openocd -d2 -s /root/.platformio/packages/tool-openocd/scripts -f board/st_nucleo_f4.cfg -c "program {/home/pi/firmware.elf}  verify reset; shutdown;"'

# end
