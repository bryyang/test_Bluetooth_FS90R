#include "BluetoothSerial.h" // biblioteca bluettoth
#include "ESP32Servo.h" // biblioteca servo

BluetoothSerial SerialBT; // clase BluetoothSerial Objeto SerialBT

// VALORES DE PARADA CALIBRADOS(EN 90 NO SE DETIENEN)   
const int PARADA_X = 89; // Calibrado para GPIO 26
const int PARADA_Y = 87; // Calibrado para GPIO 27


// Definición de Pines GPIO
const int MOTOR_X_PIN = 26; 
const int MOTOR_Y_PIN = 27; 

// Velocidades de Movimiento
const int giroHorario = 140;
const int giroAntihorario = 40;    

Servo servoX; // clase Servo Objeto servoX
Servo servoY; // clase Servo Objeto servoY

void setup() {
  Serial.begin(115200); // bd
  SerialBT.begin("ESP32_Control_Servos"); // Nombre   que aparece en la lista de Bluetooth del tlfn
  Serial.println("Bluetooth iniciado. Servomotores listos."); // Cuando el tlfn se conecta 

  // Adjuntar los pines a los objetos Servo
  servoX.attach(MOTOR_X_PIN);
  servoY.attach(MOTOR_Y_PIN);
  
  // Detener los servos usando los valores calibrados
  servoX.write(PARADA_X);
  servoY.write(PARADA_Y);
  Serial.println("Motores detenidos en valores calibrados.");
}

void loop() {
  // Comprueba si hay datos disponibles desde Bluetooth
  if (SerialBT.available()) {

    int comando = SerialBT.read();

    // Lógica de Control para Servos (Compara el carácter simple) ---
    switch (comando) {
      case 1:
        // 1: Eje X Negativo (XAccel < -2)
        servoX.write(giroHorario);
        servoY.write(PARADA_Y); // Detener el eje Y
        break;
      
      case 2:
        // 2: Eje X Positivo (XAccel > 2)
        servoX.write(giroAntihorario);
        servoY.write(PARADA_Y);
        break;
        
      case 3:
        // 3: Eje Y Negativo (YAccel < -2)
        servoY.write(giroHorario);
        servoX.write(PARADA_X); // Detener el eje X
        break;

      case 4:
        // 4: Eje Y Positivo (YAccel > 2)
        servoY.write(giroAntihorario);
        servoX.write(PARADA_X);
        break;
        
      case 5:
        // 5: DETENER
        // Detener AMBOS motores 
        servoX.write(PARADA_X);
        servoY.write(PARADA_Y);
        break;
        
      default:
        // Si se recibe un comando inválido, parar por seguridad
        servoX.write(PARADA_X);
        servoY.write(PARADA_Y);
        break;
    }
  }
}

