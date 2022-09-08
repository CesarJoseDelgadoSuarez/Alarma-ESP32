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

//Pins led 
#define ledAlarmaOn 27
#define ledAlarmaOff 33
#define ledSirena 26

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
bool passCorrecta=false;

//bool sensores
bool sensor24hDisparado = false;
// Variables de tiempo
unsigned long tiempoCuentaAtrasSalidaSegundos = 5;  // Variable que indica el tiempo para salir del domicilio tras conectar la alarma

// Metodo -> Imprimir mensaje de alarma conectada
void imprimirMensajeAlarmaconectada(String m,int fila, int columna){
  lcd.setCursor(columna,fila);
  lcd.print("                    ");
  lcd.setCursor(columna,fila);
  lcd.print(m);
} 

//Metodo -> Enciende los leds de la alarma
void ledsOn(){
  Serial.println("ledsOn()");
  digitalWrite(ledAlarmaOff,LOW);
  digitalWrite(ledAlarmaOn,HIGH);
}

//Metodo -> Apaga los leds de la alarma
void ledsOff(){
  Serial.println("ledsOff()");
  digitalWrite(ledAlarmaOff,HIGH);
  digitalWrite(ledAlarmaOn,LOW);
}

//Metodo -> Enciende la alarma
void encenderAlarma(){
  alarmaOn=true;
  //LEDS ON
  ledsOn();
  Serial.println("Encender Alarma()");
  String m;
  if (awayOn)
  {
    m = "     Alarma AWAY     ";
  }else{
    m = "     Alarma STAY     ";
  }
  
  imprimirMensajeAlarmaconectada(m,0,0);
}

// Metodo -> Enciende la alarma tras ser activado el sensor 24h
void encenderAlarma24HPresionado(){

}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 24h
void sensor24hPresionado() // FUNCION PARA H24
{
  sensor24hDisparado = true;
  alarmaOn = true;
  awayOn = true;
  stayOn = true;
  encenderAlarma24HPresionado();
  ledsOn();

}

// Metodo -> acciones que realiza ESP32 al activarse el sensor 2
void sensor2Presionado()
{

}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 3
void sensor3Presionado()
{

}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 4
void sensor4Presionado()
{

}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 5
void sensor5Presionado()
{

}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 6
void sensor6Presionado()
{

}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 7
void sensor7Presionado()
{

}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 7
void sensor8Presionado(){

}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 8
void sensor8Secuencia()
{

}

// Metodo -> interrupcion del sensor de 24h
void interrupcionSensor24h()
{
  sensor24h.read();
}

// Metodo -> interrupcion del sensor de 2
void interrupcionSensor2()
{
  sensor2.read();
}

// Metodo -> inicializa todas las varibles de EasyButton referidas a los sensores de la alarma
void setupSensores(){
  sensor24h.begin(); 
  sensor24h.onPressed(sensor24hPresionado);
  if (sensor24h.supportsInterrupt())
  {
    sensor24h.enableInterrupt(interrupcionSensor24h);
  }

  sensor2.begin();
  sensor2.onPressed(sensor2Presionado);
  if (sensor2.supportsInterrupt())
  {
    sensor2.enableInterrupt(interrupcionSensor2);
  }
  
  sensor3.begin(); 
  sensor3.onPressed(sensor3Presionado); 
  
  sensor4.begin(); 
  sensor4.onPressed(sensor4Presionado);
  
  
  sensor5.begin(); 
  sensor5.onPressed(sensor5Presionado);
  
  
  sensor6.begin();
  sensor6.onPressed(sensor6Presionado);
  
  
  sensor7.begin();
  sensor7.onPressed(sensor7Presionado);
  
  sensor8.begin(); 
  sensor8.onPressed(sensor8Presionado);
  sensor8.onSequence(2, 3000, sensor8Secuencia);

}

// Metodo -> inicializa la variable lcd
void setupLCD(){
  lcd.init();
}

// Meotodo -> inicializa las variables del teclado
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

// Metodo -> imprime la pantalla de inicio de la alarma
void pantallaInicio(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("* o # para alarma on");
}

// Metodo -> inicializa 
void setupLedsAlarma(){
  pinMode(ledAlarmaOff,OUTPUT);
  pinMode(ledAlarmaOn,OUTPUT);
  digitalWrite(ledAlarmaOff,HIGH);
  digitalWrite(ledAlarmaOn,LOW);
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

  //leds Alarma
  setupLedsAlarma();
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

// // Metodo-> resetea la contraseña introducida 
// bool resetearPass(){
//   for(int i = 0; i < sizeof(pass)/sizeof(pass[0]);i++){//sizeof() devuelve el numero de bytes total del array. para obtener cuantos elementos hay que dividir los bytes totales / byte de cada elemento
//     passIntroducida[i] = 0;
//   }
//   return true;
// }

// Metodo -> Apaga la alarma solo si la contraseña se ha introducido correctamente
void apagarAlarma(){
  alarmaOn=false;
  awayOn=false;
  stayOn=false;
  passCorrecta=false;
  ledsOff();
  pantallaInicio();
  //resetearPass();
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
        passCorrecta=false;
      }else{
        Serial.println("Contraseña Correcta!!");
        columnaAsteriscoPass=8;
        indicePassIntroducida=0;
        passCorrecta=true;
        apagarAlarma();
      }
    }
    
    esperaActiva(250);//para evitar rebote

  }
}

// Metodo-> realiza la cuenta atras dado un tiempo
void cuentaAtras(String m, unsigned long tiempoReferencia, int segundosRestantes){
  imprimirMensajeAlarmaconectada(m,0,0);
  Serial.println(m);
  for (int i = segundosRestantes; i >= 0; i--)
  {
    while (millis()-tiempoReferencia <= 1000){
      comprobarPass();
    };
    if (passCorrecta)break;
    
    Serial.println(i);
    lcd.setCursor(17,0);
    lcd.print(i);
    tiempoReferencia = millis();
  }
  tiempoReferencia = millis();
  while (millis() - tiempoReferencia <= 1000){
    comprobarPass();
    if (!passCorrecta)break;
    
  }  
}

void revisarBotonesEncendido(){
  char c = teclado.getChar();
  if (c=='*')//Alarma activada en AWAY
  {
    cuentaAtras("Alarma en AWAY:",millis(),tiempoCuentaAtrasSalidaSegundos);
    awayOn = true;
    stayOn = false;
    encenderAlarma();
  }

  if (c=='#')//Alarma activada en STAY
  {
    cuentaAtras("Alarma en STAY:",millis(),tiempoCuentaAtrasSalidaSegundos);
    awayOn = false;
    stayOn = true;
    encenderAlarma();
    
  }
  
}

void loop()
{
  if(!alarmaOn)
  {
    revisarBotonesEncendido();
  }else{
    if (sensor24hDisparado)
    {
      imprimirMensajeAlarmaconectada("   Alarma On Away   ",0,0);
    }
    
    comprobarPass();
  }
}