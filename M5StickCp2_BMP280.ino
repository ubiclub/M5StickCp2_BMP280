#include <M5StickCPlus2.h>
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include <math.h>

Adafruit_BMP280 bme;

int hosei = 0 ;

float getAltitude(float wkTemp, float wkPress) {
	float wkAltitude = 0;
	float seaAltitude = (float)1013.25 ;
	float PressJyou = (float)1 / (float)5.257 ;


	float wkPressHi = seaAltitude / wkPress ;

	float wkPress2 =  powf(wkPressHi, PressJyou) ;
	wkPress2 = wkPress2 - (float)1 ;

	float wkTemp2 = wkTemp + 273.15 ;

	wkAltitude = wkPress2 * wkTemp2 / (float)0.0065 ;

	return wkAltitude ;
}

void setup() {
	// put your setup code here, to run once:
	M5.begin();
	Wire.begin(0,26);
	M5.Lcd.setRotation(3);
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setCursor(0, 0, 2);
	M5.Lcd.println("ENV TEST");

	if (!bme.begin(0x76)){  
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	}
}

void loop() {
	// put your main code here, to run repeatedly:
	float temp, altitude, pressure, humid;

	// 補正を行う ボタン処理は再考してください
	if ( M5.BtnA.wasPressed() ) {
		// +補正
		hosei += 5 ;
	} else if (M5.BtnB.wasPressed() ) {
		// -補正
		hosei -= 5 ;
    }
    
	temp = (float)bme.readTemperature();
	pressure = (float)bme.readPressure() / 100;

	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setCursor(0, 0, 2);
	M5.Lcd.printf(" pressure: %4.0f hPa\r\n", pressure);

	altitude = getAltitude(temp, pressure) + hosei ; 
	M5.Lcd.printf(" altitude: %4.1f m\r\n", altitude);

	//humid = (float)bme.readHumidity() ;
	//M5.Lcd.printf(" humid: %3.2f \r\n", humid);
	    
	M5.Lcd.printf(" temp: %2.2f \r\n", temp);
	    
	delay(1000);
}
