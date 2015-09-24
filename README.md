# WiFI-RGBWW-module
Chine Wifi rgbww controller module hack


It's a simple openHAB configuration to control

The pinout should be 
    
    // RGB FET
    redPIN12
    greenPIN  13
    bluePIN   15
    
    // W FET
    w1PIN 14
    w2PIN 4
    
    
    // onbaord green LED D1
    LEDPIN5
    // onbaord red LED D2
    LED2PIN   1

	TX GPIO2 @Serial1 (Serial ONE)
	RX GPIO3 @Serial  


Jumper closed -> start in bootloader mode on Power on. 
Jumper open   -> start user programm 