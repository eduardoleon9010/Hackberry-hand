"""#Control de Servos Basado en Lecturas de Sensores (Arduino)
Este código en Arduino ha sido desarrollado para el control de servos a través de lecturas de sensores analógicos. Utilizando la librería Servo.h, 
  este programa opera con placas Arduino para gestionar los movimientos de los servos en función de las lecturas de sensores.

## Características Principales
Configura el comportamiento de los servos según las lecturas de un sensor analógico.
Define límites y rangos para los movimientos de los servos en respuesta a las lecturas del sensor.
Emplea funciones para la escala de valores y el cálculo de la integración necesaria para ajustar los movimientos de los servos.
Este código es parte de un proyecto más grande centrado en el desarrollo de extremidades artificiales. Está diseñado específicamente para el entorno
de programación de Arduino y se dirige a placas Arduino con sus respectivos pines y capacidades.

Este código controla los servos basados en lecturas de sensores. Utiliza una serie de funciones para ajustar los servos según la información del sensor. 
Las funciones rDeTres(), test(), y integracion() juegan roles clave en el ajuste y control de los servos. Se recomienda un entendimiento completo de la 
lógica y el propósito de estas funciones antes de realizar modificaciones significativas.

¡Siéntete libre de explorar, modificar y utilizar este código para aplicaciones relacionadas con el control de servos en entornos Arduino!
"""


#include <Servo.h> // Librería para controlar los servos

// Definición de constantes para límites y muestras
#define lim 50
#define muestras 1

// Pines para controlar los servos
#define pinServoIndex  3
#define pinServoOther 5
#define pinServoThumb 6

// Definición de ángulos máximos y mínimos para los servos
const int outThumbMax = 90; // Abierto para el pulgar
const int outIndexMax = 120; // Abierto para el dedo índice
const int outOtherMax = 150; // Abierto para los otros dedos
const int outThumbMin = 40; // Cerrado para el pulgar
const int outIndexMin = 90; // Cerrado para el dedo índice
const int outOtherMin = 90; // Cerrado para los otros dedos

// Variables para lectura de sensores
int lect = 0; // Valor leído del sensor
int otherSensorMax = 180; // Máximo valor del sensor

// Inicialización de servos
Servo servoIndex; // Servo para el dedo índice
Servo servoOther; // Servo para los otros dedos
Servo servoThumb; // Servo para el pulgar

// Variables para cálculos de control PID
uint32_t tiempo, frecuencia;
int acumulado, newVal, oldVal;
float integral;
uint8_t contMuestras;
static int accAngulo = 60;
long i;

// Prototipos de funciones
int rDeTres(int, int, int, int, int);
float integracion(float, float, float, float);
int test (int);
void imprimir();
float a = 0;
int bandera = 0;

void setup()  
{
  Serial.begin(115200); // Inicialización de la comunicación serial
  servoIndex.attach(pinServoIndex); // Asignación del pin del servo del dedo índice
  servoOther.attach(pinServoOther); // Asignación del pin del servo de los otros dedos
  servoThumb.attach(pinServoThumb); // Asignación del pin del servo del pulgar
  servoIndex.write(0); // Posición inicial del servo del dedo índice
  servoOther.write(0); // Posición inicial del servo de los otros dedos
  servoThumb.write(0); // Posición inicial del servo del pulgar
}  

void loop()   
{
  int e = analogRead(A0); // Lectura del sensor
  lect = test(e); // Llamada a la función de prueba
  if(i >= 150){
    Serial.println(e); // Impresión de la lectura del sensor
    i = 0;
  }
  i++;
  // Control de los servos según los valores del sensor
  servoIndex.write(rDeTres(lect, otherSensorMax, 0, outIndexMin, outIndexMax));
  servoOther.write(rDeTres(lect, otherSensorMax, 0, outOtherMin, outOtherMax));
  servoThumb.write(rDeTres(lect, otherSensorMax, 0, outThumbMin, outThumbMax)); 
} 

void imprimir(){
  Serial.println("=========================");
  Serial.println("=========================");
} 

// Función para escalar valores
int rDeTres(int in, int inMin, int inMax, int outMin, int outMax){
    return (in - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
 }

// Función para pruebas
int test(int valorADC){
  acumulado += valorADC / 4;
  contMuestras++;
  if(contMuestras >= muestras){
    int promediado = acumulado / muestras;

    if(promediado >= lim){
      newVal = accAngulo * 0.7;
      integral = integracion(oldVal + 1, newVal, integral, (0.5 * muestras));
    } else {
      newVal = -1 * accAngulo * 0.05;
      integral = integracion(oldVal + 1, newVal, integral, (0.05 * muestras));
    }
    if(integral < 0) integral = 0;
    if(integral > 180) integral = 180;
    oldVal = newVal;
    acumulado = 0;
    contMuestras = 0;
  }
  return integral;
}

// Función para cálculo de integración
float integracion(float previo, float actual, float sumatoria, float dt){
  if (isinf(previo) || isnan(previo)){
    previo = 0.0;
  }
  if (isinf(actual) || isnan(actual)){
    actual = 0.0;
  }
  sumatoria = sumatoria + ((actual + previo) * 0.5 * dt);
  if (isinf(sumatoria) || isnan(sumatoria)){
    return 0;
  }
  return sumatoria;
}


