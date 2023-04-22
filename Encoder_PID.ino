#include <TimerOne.h>

//Declaración de Variables
float error = 0;
float errorI = 0;
float salidaM = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pin 2 para señal de encoder
const int interruptPin = 2;   
//const int SeñalA = 10; //señal a puente h
//const int SeñalB = 9; //señal b puente h
//const int Enable = 11; //señal Enable puente H 

// Contador: cuenta las interrupciones debido a un flanco de subida en el encoder
long unsigned int counter = 0;       
     
// Variable para registrar la velocidad del motor.
double  VelocidadEnRPM = 0.0; 
// Tiempo transcurrido 
float tiempo=0.0;
// PWM
int entrada=0;                
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Configuración de Pines
void setup() {

  // El Pin interruptPin es entrada, se conecta a la señal del encoder Canal A (tren de pulsos).
  pinMode(interruptPin, INPUT);
  pinMode(10, OUTPUT);//señal A
  pinMode(11, OUTPUT);//Señal pWM
  pinMode(7, OUTPUT);//Señla B
  
  digitalWrite(10, LOW);
  digitalWrite(7, HIGH);  
  // Interrupción Encoder: cada vez que haya un flanco de subida en el interruptPin se ejecutará la
  // función counting que es una Interrupt Service Routine (ISR).

  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR_counting, RISING);  

  // Timer1: Se dispara cada 20000 microsegundos=0.02 Segundos.
  Timer1.initialize(20000); 
  // Activa la interrupción: Ejecuta la función RevolucionesPorMinuto            
  Timer1.attachInterrupt(ISR_RevolucionesPorMinuto); 
                                                                      
  //Inicializar la transmisión serial bit/s (9600)
  Serial.begin(9600);  
}

//Loop infinito
void loop() {
if (tiempo<=3)
{
entrada=0;
}
else
{
entrada=80;
}
error=entrada-VelocidadEnRPM; //calculamos error
errorI=errorI+0.02*error; // calculamos error integral
salidaM=(3*error)+(40*errorI); //modificamos la salida
if (salidaM>255) //limites reales maximos
{
salidaM=255;
}
else if (salidaM<0) //limites reales minimos
{
salidaM=0;
}
analogWrite(11,salidaM);
}
//Funcion Contador
// Se incrementa la variable counter 
//cada vez que el encoder tiene un flanco de subida

void ISR_counting() {
//Contador
counter++;
}

void ISR_RevolucionesPorMinuto(){
  // Velocidad en Revoluciones por minuto: esta parte "(counter/CuentasPorRevolucion)/Ts" 
  // da las revoluciones por segundo a las que gira el encoder, 
  // se multiplica por 60 segundos para obtener esta velocidad en RPM
  VelocidadEnRPM = 60*counter/(12*34*0.02);   //pulsos por revolucio,relacion del moto reductor,tiempo

                              
  tiempo = tiempo + 0.02;                                                                                                                                           

  // Se imprime el valor del DutyCycle y de la velocidad del motor en RPM
  Serial.println(VelocidadEnRPM);

  // Se reinicia el contador para que vuelva a contar hasta la siguiente interrupción.
  counter = 0;                                                            
}
