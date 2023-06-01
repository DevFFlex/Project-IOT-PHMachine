Note1: You should make sure you are using OPEN-SMART 3.5 INCH TFT LCD Breakout whose driver IC is RM68140.
Note2: Plese make sure uncomment the code (#define SUPPORT_9488//for RM68140, ILI9488 ) in the head of the file MCUFRIEND_kbv.cpp.
Note3: Because the resolution of the shield is 320*480, the code in the MCUFRIEND_kbv.cpp should also be like that:
MCUFRIEND_kbv::MCUFRIEND_kbv(int CS, int RS, int WR, int RD, int RST):Adafruit_GFX(320, 480)
{
    // we can not access GPIO pins until AHB has been enabled.
}