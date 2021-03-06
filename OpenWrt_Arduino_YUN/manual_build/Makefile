
AVR_PATH ?= /usr/bin

AVRFLAGS = -g -mmcu=atmega32u4
AVRCFLAGS = -Os -w -ffunction-sections -fdata-sections -MMD
AVRCPFLAGS = -fno-exceptions -fno-threadsafe-statics $(AVRCFLAGS)
AVRGCC = $(AVR_PATH)/avr-gcc $(AVRFLAGS) -std=gnu11 $(AVRCFLAGS)
AVRGPP = $(AVR_PATH)/avr-g++ $(AVRFLAGS) -std=gnu++11 $(AVRCFLAGS)
AVRASM = $(AVR_PATH)/avr-gcc $(AVRFLAGS) -x assembler-with-cpp


GPPCORES = $(wildcard core/*.cpp)
GCCCORES = $(wildcard core/*.c)
ASMCORES = $(wildcard core/*.S)

COREOBJS = $(GPPCORES:.cpp=.o)  $(GCCCORES:.c=.o)  $(ASMCORES:.S=.asm.o)
COREDEF = -DF_CPU=16000000L -DARDUINO=10606 -DARDUINO_AVR_YUN -DARDUINO_ARCH_AVR -DUSB_VID=0x2341 -DUSB_PID=0x8041 -DUSB_MANUFACTURER="Unknown" -DUSB_PRODUCT=\"Arduino\ Yun\"
COREINC = -Icore -Iyun


BASENAME ?= minimum
TARGET = $(BASENAME).elf
EEPROM = $(BASENAME).eep $(BASENAME).hex

$(TARGET): $(BASENAME).cpp core.a
	$(AVRGPP) $(COREDEF) $(COREINC) $(BASEINC) -Wl,--gc-sections $< core.a -L./ -lm -o $@
	@echo -e '\e[0;32mTarget for $< complete:'
	-$(AVR_PATH)/avr-size  $@ --format=avr --radix=10
	@echo -e '\e[0m'



core.a: $(COREOBJS)
	$(AVR_PATH)/avr-ar rcs core.a $(COREOBJS)

core/%.o: core/%.cpp
	@echo -e '\e[0;32mGPP $<\e[0m'
	$(AVRGPP) -c $(COREDEF) $(COREINC) $< -o $@

core/%.o: core/%.c
	@echo -e '\e[0;32mGCC $<\e[0m'
	$(AVRGCC) -c $(COREDEF) $(COREINC) $< -o $@

core/%.asm.o: core/%.S
	@echo -e '\e[0;32mASM $<\e[0m'
	$(AVRASM) -c $(COREDEF) $(COREINC) $< -o $@

-include $(GPPCORES:.cpp=.d)  $(GCCCORES:.c=.d)  $(BASENAME).d



upload: $(BASENAME).hex
	/bin/stty -F /dev/ttyACM0 1200 ; sleep 1 ; avrdude -p atmega32u4 -c avr109 -P /dev/ttyACM0 -b 57600 -D -U flash:w:$<:i
	
$(EEPROM): $(TARGET)
	$(AVR_PATH)/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  $(TARGET) $(BASENAME).eep 
	$(AVR_PATH)/avr-objcopy -O ihex -R .eeprom  $(TARGET)  $(BASENAME).hex



clean:
	rm -f $(TARGET) $(EEPROM) core.a core/*.o core/*.d $(BASENAME).o $(BASENAME).d

