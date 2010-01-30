# Super Makefile to build or clean all the subprojects.
# The -C argument to make tells it to enter a subdirectory and run the following target there.
# The '@' character in front of the echo commands prevents the command itself from being displayed.

all:
	make -C BlinkLED
	make -C TestADC
	make -C TestDigital
	make -C TestI2C
	make -C TestLCD
	make -C TestLEDArray
	make -C TestMotors
	make -C TestRTC
	make -C TestServo
	make -C TestSpecialLCDChars
	@echo "Done making!"

clean:
	make -C BlinkLED clean
	make -C TestADC clean
	make -C TestDigital clean
	make -C TestI2C clean
	make -C TestLCD clean
	make -C TestLEDArray clean
	make -C TestMotors clean
	make -C TestRTC clean
	make -C TestServo clean
	make -C TestSpecialLCDChars clean
	@echo "Done cleaning!"
