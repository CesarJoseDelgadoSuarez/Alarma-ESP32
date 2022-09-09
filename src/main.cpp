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
bool sistemaOk = true;
bool sistemaEstabaOk=true;
//bool sensores
bool sensor24hDisparado = false;


bool sensor2DisparadoAlarmaOff = false;
bool sensor3DisparadoAlarmaOff = false;
bool sensor4DisparadoAlarmaOff = false;
bool sensor5DisparadoAlarmaOff = false;
bool sensor6DisparadoAlarmaOff = false;
bool sensor7DisparadoAlarmaOff = false;
bool sensor8DisparadoAlarmaOff = false;

bool sensor2DisparadoAlarmaOn = false;
bool sensor3DisparadoAlarmaOn = false;
bool sensor4DisparadoAlarmaOn = false;
bool sensor5DisparadoAlarmaOn = false;
bool sensor6DisparadoAlarmaOn = false;
bool sensor7DisparadoAlarmaOn = false;
bool sensor8DisparadoAlarmaOn = false;

bool mensajeSensor24hDisparado = false;

// Variables de tiempo
unsigned long tiempoCuentaAtrasSalidaSegundos = 5;  // Variable que indica el tiempo para salir del domicilio tras conectar la alarma
unsigned long sirenaEncendida = false;              // Variable que indica el momento en el que se encendio la sirena
// Metodo -> Imprimir mensaje de alarma conectada
void imprimirMensajeLCD(String m,int fila, int columna){
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


//Metodo -> Enciende la sirena de la alarma
void sirenaOn(){
  digitalWrite(ledSirena,HIGH);
  sirenaEncendida = millis();
}

//Metodo -> Apaga la sirena de la alarma
void sirenaOff(){
  digitalWrite(ledSirena,LOW);
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
  
  imprimirMensajeLCD(m,0,0);
}

// Metodo -> acciones que realiza ESP32 al activarse el sensor 24h
void sensor24hPresionado() // FUNCION PARA H24
{
  sensor24hDisparado = true;
  alarmaOn = true;
  awayOn = true;
  stayOn = true;
  ledsOn();
  sirenaOn();
}

// Metodo -> acciones que realiza ESP32 al activarse el sensor 2
void sensor2Presionado()
{
  if (!alarmaOn)
  {
    sensor2DisparadoAlarmaOff=true;
  }else{
    sensor2DisparadoAlarmaOn=true;
  }
  
}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 3
void sensor3Presionado()
{
  if (!alarmaOn)
  {
    sensor3DisparadoAlarmaOff=true;
  }else{
    sensor3DisparadoAlarmaOn=true;
    sirenaOn();
  }
  
}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 4
void sensor4Presionado()
{
  if (!alarmaOn)
  {
    sensor4DisparadoAlarmaOff=true;
  }else{
    sirenaOn();
    sensor4DisparadoAlarmaOn=true;
  }
  
}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 5
void sensor5Presionado()
{
  if (!alarmaOn)
  {
    sensor5DisparadoAlarmaOff=true;
  }else{
    sirenaOn();
    sensor5DisparadoAlarmaOn=true;
  }
  
}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 6
void sensor6Presionado()
{
  if (!alarmaOn)
  {
    sensor6DisparadoAlarmaOff=true;
  }else{
    sirenaOn();
    sensor6DisparadoAlarmaOn=true;
  }
  
}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 7
void sensor7Presionado()
{
  if (!alarmaOn)
  {
    sensor7DisparadoAlarmaOff=true;
  }else{
    sirenaOn();
    sensor7DisparadoAlarmaOn = true;
  }
}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 7
void sensor8Presionado(){
  if (!alarmaOn)
  {
    sensor8DisparadoAlarmaOff=true;
  }
}
// Metodo -> acciones que realiza ESP32 al activarse el sensor 8
void sensor8Secuencia()
{
  sirenaOn();
  if(alarmaOn)sensor8DisparadoAlarmaOn=true;
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
  lcd.backlight();
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
  pinMode(ledSirena,OUTPUT);
  digitalWrite(ledAlarmaOff,HIGH);
  digitalWrite(ledAlarmaOn,LOW);
  digitalWrite(ledSirena,LOW);

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

//Metodo -> resetea las variables que deben resetearse cuando se apaga la alarma
void resetVariablesAlarmaOff(){
  sensor2DisparadoAlarmaOn = false;
  sensor3DisparadoAlarmaOn = false;
  sensor4DisparadoAlarmaOn = false;
  sensor5DisparadoAlarmaOn = false;
  sensor6DisparadoAlarmaOn = false;
  sensor7DisparadoAlarmaOn = false;
  sensor8DisparadoAlarmaOn = false;
}

// Metodo -> Apaga la alarma solo si la contraseña se ha introducido correctamente
void apagarAlarma(){
  alarmaOn=false;
  awayOn=false;
  stayOn=false;
  passCorrecta=false;
  ledsOff();
  pantallaInicio();
  mensajeSensor24hDisparado = false;

  resetVariablesAlarmaOff();
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
      esperaActiva(500);
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
    
    esperaActiva(200);//para evitar rebote

  }
}

// Metodo-> realiza la cuenta atras dado un tiempo
void cuentaAtras(String m, unsigned long tiempoReferencia, int segundosRestantes){
  imprimirMensajeLCD(m,0,0);
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

// Metodo -> Estando la alarma desconectada mira que botones se pulsan y actua en consecuencia
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

  if (c=='D')//Mostrar zonas disparadas ultima conexion
  {
    //Pendiente
  }
  
  
}

//Metodo -> muestra las zonas activadas cuando la alarma no esta conectada y no deja conectar la alarma mientras hayan zonas activas mas de un tiempo x
void verZonasActivasAlarmaOff(){
  if (!sensor2.isPressed())
  {
    if (sensor2DisparadoAlarmaOff)
    {
      lcd.setCursor(3,2);
      lcd.print("Z2");
    }
  }else{
    lcd.setCursor(3,2);
    lcd.print("  ");
    sistemaOk = true;
    sensor2DisparadoAlarmaOff=false;
  }

  if (!sensor3.isPressed())
  {
    if (sensor3DisparadoAlarmaOff)
    {
      lcd.setCursor(6,2);
      lcd.print("Z3");
    }
  }else{
    lcd.setCursor(6,2);
    lcd.print("  ");
    sistemaOk = true;
    sensor3DisparadoAlarmaOff=false;
  }
  
  if (!sensor4.isPressed())
  {
    if (sensor4DisparadoAlarmaOff)
    {
      lcd.setCursor(9,2);
      lcd.print("Z4");
    }
  }else{
    lcd.setCursor(9,2);
    lcd.print("  ");
    sistemaOk = true;
    sensor4DisparadoAlarmaOff=false;
  }

  if (!sensor5.isPressed())
  {
    if (sensor5DisparadoAlarmaOff)
    {
      lcd.setCursor(12,2);
      lcd.print("Z5");
    }
  }else{
    lcd.setCursor(12,2);
    lcd.print("  ");
    sistemaOk = true;
    sensor5DisparadoAlarmaOff = false;
  }
  
  if (!sensor6.isPressed())
  {
    if (sensor6DisparadoAlarmaOff)
    {
      lcd.setCursor(15,2);
      lcd.print("Z6");
    }
    
  }else{
    lcd.setCursor(15,2);
    lcd.print("  ");
    sistemaOk = true;
    sensor6DisparadoAlarmaOff = false;
  }
  
  if (!sensor7.isPressed())
  {
    if (sensor7DisparadoAlarmaOff)
    {
      lcd.setCursor(18,2);
      lcd.print("Z7");
    }
  }else{
    lcd.setCursor(18,2);
    lcd.print("  ");
    sistemaOk = true;
    sensor7DisparadoAlarmaOff = false;
  }

  if (!sensor8.isPressed())
  {
    if (sensor8DisparadoAlarmaOff)
    {
      lcd.setCursor(9,3);
      lcd.print("Z8");
    }
  }else{
    lcd.setCursor(9,3);
    lcd.print("  ");
    sistemaOk = true;
    sensor8DisparadoAlarmaOff=false;
  }
  
  if (sensor2.releasedFor(1500) || sensor3.releasedFor(1500) || sensor4.releasedFor(1500) || sensor5.releasedFor(1500) || sensor6.releasedFor(1500) || sensor7.releasedFor(1500) || sensor8.releasedFor(1500))
  {
    if(sistemaEstabaOk)imprimirMensajeLCD("Fallo en Sistema",0,0);
    sistemaEstabaOk=false;
    sistemaOk = false;
  }

  
}

// Metodo -> muestra las zonas activadas cuando la alarma esta conectada
void verZonasActivasAlarmaOn(){
  if (sensor24hDisparado)
  {
    lcd.setCursor(0,2);
    lcd.print("Z1");
  }

  if (sensor2DisparadoAlarmaOn)
  {
    lcd.setCursor(3,2);
    lcd.print("Z2");
  }

  if (sensor3DisparadoAlarmaOn)
  {
    
    lcd.setCursor(6,2);
    lcd.print("Z3");
  }
  
  if (sensor4DisparadoAlarmaOn)
  {
    lcd.setCursor(9,2);
    lcd.print("Z4");
  }

  if (sensor5DisparadoAlarmaOn)
  {
    lcd.setCursor(12,2);
    lcd.print("Z5");
  }
  
  if (sensor6DisparadoAlarmaOn)
  {
    lcd.setCursor(15,2);
    lcd.print("Z6");    
  }
  
  if (sensor7DisparadoAlarmaOn)
  {
    lcd.setCursor(18,2);
    lcd.print("Z7");
  }

  if (sensor8DisparadoAlarmaOn)
  {
    lcd.setCursor(9,3);
    lcd.print("Z8");
  }
}

// Metodo -> lee las zonas estando la alarma desconectada
void leerZonas(){
  sensor2.read();
  sensor3.read();
  sensor4.read();
  sensor5.read();
  sensor6.read();
  sensor7.read();
  sensor8.read();
}

//Metodo -> lee las zonas correspondientes dependiendo del modo de la alarma STAY/AWAY
void readZonas(){
  sensor3.read();
  sensor4.read();
  sensor5.read();
  if (awayOn)
  {
    sensor6.read();
    sensor7.read();
    sensor8.read();
  }
}

// Metodo -> Apaga la sirena si lleva un tiempo encendida
void apagarSirenaSi(){
  if (millis() - sirenaEncendida >= 5000)sirenaOff();
}

void loop()
{
  if(!alarmaOn)
  {
    leerZonas();
    verZonasActivasAlarmaOff();
    if(sistemaOk){
      revisarBotonesEncendido();
      if(!sistemaEstabaOk)pantallaInicio();sistemaEstabaOk=true;
      
    }
  }else{
    if (sensor24hDisparado && !mensajeSensor24hDisparado){
      imprimirMensajeLCD("     Alarma AWAY     ",0,0);
      mensajeSensor24hDisparado = true;
    }
    verZonasActivasAlarmaOn();
    comprobarPass();
    readZonas();
    apagarSirenaSi();
  }
}