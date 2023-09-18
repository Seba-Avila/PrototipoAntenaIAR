#include <Arduino.h>
#include <Servo.h>

int cen;
int dec;
int uni;
int gradoB;
int gradoS;
char cache;
int flag=0;
String receivedData = "";

Servo servoBase;
Servo servoSup;

void LimpiarMenu();
/*void Grados();*/
void Comunicacion();

void setup(){
  Serial.begin(115200);

  servoBase.attach(9);  // declaramos la salidad en el pin 9
  servoSup.attach(10);  // declaramos la salidad en el pin 10

  servoBase.write(0);   // Iniciamos el servo base en 0 
  servoSup.write(0);    // Iniciamos el servo superior en 0 

  LimpiarMenu();
  Serial.println("Bienvenido\n\n");

  delay(3000);

  Comunicacion();
}

void loop(){
  servoBase.write(gradoB);
  servoSup.write(gradoS);
  
  Serial.print("Acimut: ");
  Serial.println(gradoB);
  Serial.print("Altitud: ");
  Serial.println(gradoS);
  Comunicacion();
}

void Comunicacion(){
  flag=1;
  while(flag==1){
    Serial.println("Ingrese valor acimut (0° a 360°):");
      if(Serial.available()){
        char receivedChar = Serial.read();

        if (receivedChar == '\n'){
          flag = 0;
        } else {
          receivedData += receivedChar;
        }
      }
  }
  gradoB= receivedData.toInt();
  receivedData = "";
  flag=1;
  while(flag==1){
    Serial.println("Ingrese valor altitud (0° a 90°):");
      if(Serial.available()){
        char receivedChar = Serial.read();

        if (receivedChar == '\n'){
          flag = 0;
        } else {
          receivedData += receivedChar;
        }
      }
  }
  gradoS= receivedData.toInt();
  receivedData = "";

  if(gradoB>180){
    gradoB=gradoB-180;
    gradoS=180-gradoS;
  }
}

/*void Grados(){
  LimpiarMenu();
  Serial.println("Ingrese valores de azimut (por favor, en 3 cifras): ");
  
  while(Serial.available() < 3){
    delay(3000);
    Serial.println("Esperando...");
    Serial.println((int) Serial.available());
  }
  
  gradoB=Serial.parseInt();
  cache=Serial.read();
  cache=Serial.read();
  Serial.println(gradoB);

  Serial.println("Ingrese valores de altitud (por favor, en 3 cifras): ");
  while(Serial.available() < 3){
    delay(3000);
    Serial.println("Esperan2...");
    Serial.println((int) Serial.available());
  }
  gradoS=Serial.parseInt();
  cache=Serial.read();
  cache=Serial.read();
  Serial.println(gradoS);
}*/

void LimpiarMenu(){
  for(int i=0; i<18; i++){
    Serial.println("\n");
  }
}