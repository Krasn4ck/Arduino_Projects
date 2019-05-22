﻿/* VERSION 0.1.2 BETHA CON SENSOR DE AGUA
 * PRÁCTICA 6 CONTROL DE AGUA CON ARDUINO
 * **************************************
 * --COMPONENTES A UTILIZAR EN EL CIRCUITO--  
 * - OPTOACOPLADOR MOC3041
 * - TRIAC BTA16-600B
 * - RESISTENCIAS DE 3330 OHMS
 * - ARDUINO NANO
 * - DISPLAY LCD 16X2
 * - SENSOR ULTRASONICO HC-SR04
 * - BOMBA DE PECERA 110V
 * - BUZZER
 * **************************************
 * PARA LA REALIZACION DEL CIRCUITO NECESITAMOS ALGUNAS LIBRERIAS
 * PARA EL DISPLAY LCD NECESITAMOS LIQUIDCRYSTAL.H
 * PARA EL SENSOR ULTRASONICO NECESITAMOS NEWPING.H .
 */
#include<LiquidCrystal.h>
#include<NewPing.h>
/*
 * DEFINIMOS CON QUE PUERTOS VAMOS A TRABAJAR CON EL ARDUINO PARA ESTO
 * NECESITAMOS DEIFINIR LOS PINES DEL SENSOR ULTRASONICO Y PARA EL LCD
 */
#define TRIGGER_PIN 10
#define ECHO_PIN 9
#define MAX_DISTANCE 200
#define ROW 16
#define COL 2
#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2
#define MAX_CISTERNA 7
#define MIN_CISTERNA 3
#define CTRL_OPTO 8
#define BUZZER 6
#define SENSOR_AGUA A0
int nivel_agua=0;
double distancia_actual=0;
/*
 * AHORA LO QUE NECESITAMOS ES CARGAR LOS PINES A LAS LIBRERIAS PARA PODER INSTANCIAR
 * LOS METODOS DE LAS LIBRERIAS
 */
 NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
 LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
void setup() {
  // put your setup code here, to run once:
/*
 * AHORA INICIALIZAMOS NUESTROS PINES YA QUE SOLO SE NECESITA CARGAR EL PIN DEL OPTOACOPLADOR
 * Y EL DISPLAY SOLO NECESITAMOS DEFINIR EL TAMAÑO DE LA MATRIZ
 */
 lcd.begin(16,2);
 pinMode(CTRL_OPTO,OUTPUT);
 pinMode(BUZZER,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
   * YA AQUI ES HORA DE CREAR EL PARAMETRO PARA QUE EL OPTOACOPLADOR JUNTO CON EL TRIAC 
   * ACTIVEN LA BOMBA, LO MAS COMUN ES UTILIZAR UNA ESTRUCTURA DE CONTROL CICLICA LLAMADA 'WHILE' YA QUE MANTENDRA
   * EL ENCENDIDO EL MOTOR MIENTRAS NO SE CUMPLA LA CONDICION SEGUIRA EJECUTANDOLO DE FORMA INDETERMINADA
   */
   digitalWrite(CTRL_OPTO,LOW);
   lcd.display();
   lcd.clear();
   lcd.setCursor(4,0);
   lcd.print("CONTROL");
   lcd.setCursor(3,1);
   lcd.print("CISTERNA");
   delay(1000);
   nivel_agua=analogRead(SENSOR_AGUA);
   nivel_agua=map(nivel_agua,0,680,0,100);
   distancia_actual=sonar.ping_cm();
   if(nivel_agua>=50)
   {
     while(distancia_actual>MAX_CISTERNA)
     {
       /*
       * EN ESTA ULTIMA PARTE  INCORPORAREMOS EL MENSAJE DE LA CISTERNA LLENA
       * Y APAGAMOS LA SALIDA DEL OPTOACOPLADOR PARA QUE NO SE DESBORDE EL TANQUE
       * ;Y EL MOTOR NO SE SOBRECALIENTE. 
       */
      
      if(distancia_actual<MIN_CISTERNA)
      { 
        digitalWrite(CTRL_OPTO,LOW);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("TANQUE LLENO WE");
        digitalWrite(BUZZER,HIGH);
        delay(1000);
        digitalWrite(BUZZER,LOW);
        lcd.noDisplay();
        delay(1000);
      }
     
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("BOMBA DE AGUA - VACIO.");
        delay(500);
        lcd.setCursor(0,1);
        lcd.print("LLENANDO.");
        delay(500);
        lcd.print(".");
        delay(500);
        lcd.print(".");
        delay(500);
        distancia_actual=sonar.ping_cm();
        digitalWrite(CTRL_OPTO,HIGH);
      
    }
   }
   if(nivel_agua<15)
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("NO HAY AGUA.");
    digitalWrite(BUZZER,HIGH);
    delay(500);
    digitalWrite(BUZZER,LOW);
    delay(500);
    digitalWrite(BUZZER,HIGH);
    delay(500);
    digitalWrite(BUZZER,LOW);
    delay(500);
   }

}
