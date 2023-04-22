#include <TimerOne.h>

//Declaración de Variables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pin 2 para señal de encoder
const int interruptPin = 2;    

// Contador: cuenta las interrupciones debido a un flanco de subida en el encoder
long unsigned int counter = 0;       
     
// Variable para registrar la velocidad del motor.
double  VelocidadEnRPM = 0.0; 
// Tiempo transcurrido 
float tiempo=0.0;                    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Configuración de Pines
void setup() {

  // El Pin interruptPin es entrada, se conecta a la señal del encoder Canal A (tren de pulsos).
  pinMode(interruptPin, INPUT);    
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
  VelocidadEnRPM = 60*counter/(12*34*0.02);   
                              
  tiempo = tiempo + 0.02;                                                                                                                                           

  // Se imprime el valor del DutyCycle y de la velocidad del motor en RPM
  Serial.println(VelocidadEnRPM);

  // Se reinicia el contador para que vuelva a contar hasta la siguiente interrupción.
  counter = 0;                                                            
}
