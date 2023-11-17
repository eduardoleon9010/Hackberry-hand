Control de Mano Biónica
Este proyecto proporciona un código para controlar una mano biónica utilizando Arduino y servomotores.

Índice
Instalación
Uso
Contribución
Licencia
Instalación
Para utilizar este código, sigue estos pasos:

Conecta los servomotores a los pines definidos en el código.
Carga el código en una placa Arduino compatible.
Asegúrate de tener configurado correctamente el sensor que se utiliza para el control de la mano biónica.
Uso
El código controla la apertura y cierre de los dedos de la mano biónica en función de las lecturas de un sensor conectado a la entrada analógica A0. Para ajustar los límites de apertura y cierre de los dedos, se han definido constantes (outThumbMax, outIndexMax, outOtherMax, outThumbMin, outIndexMin, outOtherMin) que puedes modificar según tus necesidades.

Recuerda ajustar la configuración del sensor para adaptarla al entorno en el que se utilizará la mano biónica.

Contribución
Si deseas contribuir a este proyecto, puedes seguir estos pasos:

Haz un fork del repositorio.
Realiza tus cambios en una nueva rama (git checkout -b feature/Mejora).
Realiza pruebas exhaustivas de tus cambios.
Envía tus cambios como un pull request explicando detalladamente las mejoras implementadas.
#include <Servo.h> // Librería para controlar los servos

Licencia
Este proyecto se encuentra bajo la licencia MIT License. 



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
