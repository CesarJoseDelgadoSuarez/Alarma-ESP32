#include <Arduino.h>
#include <EasyButton.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "I2CKeyPad.h"

//PANTALLA CRISTAL LIQUIDO
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//TECLADO
const uint8_t direccionTeclado = 0x20;
I2CKeyPad teclado(direccionTeclado);
char mapaTeclado[19] = "DCBA#9630852*741NF";  // N = NoKey, F = Fail

//contraseña
int pass[] ={1,2,3,4};
int passIntroducida[]={0,0,0,0};
int indicePassIntroducida = 0;
int columnaAsteriscoPass = 8;

// Pins a los que los sensores estan conectados.
#define pinSensor24h 14
#define pinSensor2 16
#define pinSensor3 17
#define pinSensor4 18
#define pinSensor5 19
#define pinSensor6 23
#define pinSensor7 13
#define pinSensor8 25

// Instancias de los sensores.
EasyButton sensor24h(pinSensor24h);
EasyButton sensor2(pinSensor2);
EasyButton sensor3(pinSensor3);
EasyButton sensor4(pinSensor4);
EasyButton sensor5(pinSensor5);
EasyButton sensor6(pinSensor6);
EasyButton sensor7(pinSensor7);
EasyButton sensor8(pinSensor8);

// Variables booleanas
bool alarmaOn = false;  // Variable que indica si la alarma esta conectada o no
bool awayOn = false;    // Variable que indica si la alarma esta conectada en AWAY
bool stayOn = false;    // Variable que indica si la alarma esta conectada en STAY



// Variables de tiempo
unsigned long tiempoCuentaAtrasSalidaSegundos = 5;  // Variable que indica el tiempo para salir del domicilio tras conectar la alarma

// Metodo-> acciones que realiza ESP32 al activarse el sensor 24h
void sensor24hPresionado()
{

}
// Metodo-> acciones que realiza ESP32 al activarse el sensor 2
void sensor2Presionado()
{

}
// Metodo-> acciones que realiza ESP32 al activarse el sensor 3
void sensor3Presionado()
{

}
// Metodo-> acciones que realiza ESP32 al activarse el sensor 4
void sensor4Presionado()
{

}
// Metodo-> acciones que realiza ESP32 al activarse el sensor 5
void sensor5Presionado()
{

}
// Metodo-> acciones que realiza ESP32 al activarse el sensor 6
void sensor6Presionado()
{

}
// Metodo-> acciones que realiza ESP32 al activarse el sensor 7
void sensor7Presionado()
{

}
// Metodo-> acciones que realiza ESP32 al activarse el sensor 8
void sensor8Secuencia()
{

}

// Metodo-> interrupcion del sensor de 24h
void interrupcionSensor24h()
{
  sensor24h.read();
}

// Metodo-> interrupcion del sensor de 2
void interrupcionSensor2()
{
  sensor2.read();
}

// Metodo-> inicializa todas las varibles de EasyButton referidas a los sensores de la alarma
void setupSensores(){
  //SENSOR 24 HORAS
  sensor24h.begin();
  sensor24h.onPressed(sensor24hPresionado);
  if (sensor24h.supportsInterrupt())
  {
    sensor24h.enableInterrupt(interrupcionSensor24h);
    Serial.println("Sensor24h habilitado para interrupciones");
  }

  //SENSOR 2
  sensor2.begin();
  sensor2.onPressed(sensor2Presionado);
  if (sensor2.supportsInterrupt())
  {
    sensor2.enableInterrupt(interrupcionSensor2);
    Serial.println("sensor2 habilitado para interrupciones");
  }

  //SENSOR 3
  sensor3.begin();
  sensor3.onPressed(sensor3Presionado);

  //SENSOR 4
  sensor4.begin();
  sensor4.onPressed(sensor4Presionado);

  //SENSOR 5
  sensor5.begin();
  sensor5.onPressed(sensor5Presionado);

  //SENSOR 6
  sensor6.begin();
  sensor6.onPressed(sensor6Presionado);

  //SENSOR 7
  sensor7.begin();
  sensor7.onPressed(sensor7Presionado);

  //SENSOR 8
  sensor8.begin();
  sensor8.onSequence(2,3000,sensor8Secuencia);
  //  sensor24h.onSequence(2, 1500, sequenceEllapsed);
}

// Metodo-> inicializa la variable lcd
void setupLCD(){
  lcd.init();
  lcd.backlight();
}

// Meotodo-> inicializa las variables del teclado
void setupTeclado(){
  Wire.begin();
  Wire.setClock(400000);
  if (teclado.begin() == false)
  {
    Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
    while (1);
  }

  teclado.loadKeyMap(mapaTeclado);
}

// Metodo-> imprime la pantalla de inicio de la alarma
void pantallaInicio(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("* o # para alarma on");
}

void setup()
{
  // Initialize Serial for debuging purposes.
  Serial.begin(115200);

  //LCD
  setupLCD();

  // EASYBUTTON.
  setupSensores();

  //Keypad
  setupTeclado();

  //pantalla inicio lcd
  pantallaInicio();
}

// Metodo-> realiza una espera activa de un tiempo t para seguir escuchando interrupciones
void esperaActiva(unsigned long tiempoEspera){
  unsigned long t = millis();
  while (millis() - t <= tiempoEspera);
}
// Metodo-> devuelve el numero pulsado, en caso contrario devuelve E
char leerSoloNumeros(){
  char c = teclado.getChar();
  if (c=='N' || c=='F' || c=='*' || c=='#'  || c=='A' || c=='B' || c=='C' || c=='D')
  {
    return 'E';
  }
  return c;
}

// Metodo-> compara la contraseña introducida para saber si se ha introducido correctamente
bool compararPass(){
  for(int i = 0; i < sizeof(pass)/sizeof(pass[0]);i++){//sizeof() devuelve el numero de bytes total del array. para obtener cuantos elementos hay que dividir los bytes totales / byte de cada elemento
    if(pass[i]!=passIntroducida[i]){
      return false;
    }
  }
  return true;
}

// Metodo-> comprueba si se ha introducido la contraseña
void comprobarPass(){
  char c = leerSoloNumeros();
  if (c != 'E')
  {
    Serial.print("Se ha pulsado un numero: ");
    int n =  c - '0';
    Serial.println(n);
    passIntroducida[indicePassIntroducida] = n ;
    lcd.setCursor(columnaAsteriscoPass,1);
    columnaAsteriscoPass++;
    indicePassIntroducida++;
    lcd.print('*');
    if (indicePassIntroducida==4)
    {
      if(!compararPass()){
        Serial.println("Contraseña Incorrecta!!");
        lcd.setCursor(8,1);
        lcd.print("    ");
        columnaAsteriscoPass=8;
        indicePassIntroducida=0;

      }else{
        Serial.println("Contraseña Correcta!!");
        columnaAsteriscoPass=8;
        indicePassIntroducida=0;
        alarmaOn=false;
        awayOn=false;
        stayOn=false;
        pantallaInicio();
      }
    }
    
    esperaActiva(250);//para evitar rebote

  }
}

// Metodo-> realiza la cuenta atras dado un tiempo
void cuentaAtras(String m, unsigned long tiempoReferencia, int segundosRestantes){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(m);
  Serial.println(m);
  for (int i = segundosRestantes; i >= 0; i--)
  {
    while (millis()-tiempoReferencia <= 1000){
      comprobarPass();
    };//dentro debe mirar si se introduce contraseña para cancelar encendido de la alarma
    if (!alarmaOn)break;
    
    Serial.println(i);
    lcd.setCursor(17,0);
    lcd.print(i);
    tiempoReferencia = millis();
  }
  tiempoReferencia = millis();
  while (millis() - tiempoReferencia <= 1000){
    comprobarPass();
    if (!alarmaOn)break;
    
  }
  if(alarmaOn){
    lcd.setCursor(0,0);
    lcd.print("  Alarma Conectada  ");
  }
  
}

void revisarBotonesEncendido(){
  char c = teclado.getChar();
  if (c=='*')//Alarma activada en AWAY
  {
    alarmaOn = true;
    awayOn = true;
    stayOn = false;
    cuentaAtras("Alarma en AWAY:",millis(),tiempoCuentaAtrasSalidaSegundos);
    
  }

  if (c=='#')//Alarma activada en STAY
  {
    alarmaOn = true;
    awayOn = false;
    stayOn = true;
    cuentaAtras("Alarma en STAY:",millis(),tiempoCuentaAtrasSalidaSegundos);
  }
  
}

void loop()
{
  if(!alarmaOn)
  {
    revisarBotonesEncendido();
  }else{
    comprobarPass();
  }
}