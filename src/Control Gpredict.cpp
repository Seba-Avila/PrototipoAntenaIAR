#include <SPI.h>
#include <EthernetENC.h>
#include "Servo.h"

#define PINSS 4
#define PINRESET 3


// Establecemos una dirección MAC
byte mac[] = {
 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192,168,1,177);

// Definimos el objeto servidor designándole el puerto 1024

#define PORT_GPREDICT 1024
EthernetServer server(PORT_GPREDICT);




Servo altitud; //Defino mi objeto servo para el motor superior 
Servo azimut;  //Defino mi objeto servo para el motor base
int posicion_az = 90; //Declaro variable para posicion azimut
int posicion_alt = 90; //Declaro variable para posicion altitud


char data[50];  //Vector para los datos recibidos
int i = 0;
String data_str, data_az, data_alt; //strings donde se guardaran los datos recibidos (string principal y substrings para datos de azimut y altitud)


void setup() {
 Serial.begin(9600);
 altitud.attach(10); //Mi motor de altitud estará conectado al pin 9
 azimut.attach(9); //Mi motor de azimut estará conectado al pin 10


 /*Inicia en posición cenit*/
 altitud.write(posicion_alt);
 azimut.write(posicion_az);


 Ethernet.init(PINSS);  //Configuro el pin 4 como pin CS


  // Open serial communications and wait for port to open:
  // Abro la comunicación serial y espero a que el puerto sea abierto:
  while (!Serial) {
   ; // Espero la conexión del puerto serial
 }
 Serial.println("Ethernet Server");


 // Inicio la conexión Ethernet con mi mac e ip:
 Ethernet.begin(mac, ip);


 // Chequeo el Hardware
 if (Ethernet.hardwareStatus() == EthernetNoHardware) {
   Serial.println("Ethernet shield no encontrado.  No se puede ejecutar sin Hardware. :(");
   while (true) {
     delay(1); 
   }
 }
 if (Ethernet.linkStatus() == LinkOFF) {
   Serial.println("Cable Ethernet no conectado.");
 }


 // Inicio el servidor
 server.begin();
 Serial.print("Servidor está en ");
 Serial.println(ip);
}




void loop() {
 // Espero la aparición de clientes
 EthernetClient client = server.available();
 if (client) {
   Serial.println("new client");
   /*// una solicitud HTTP termina con un blank line
   bool currentLineIsBlank = true;*/
   while (client.connected()) {    //Mientras haya un cliente conectado
     while (client.available()) {  //Al recibir datos del cliente
       char c = client.read();     //Guardo la lectura como único caracter
       Serial.print(c);
       if (c == 'p') {  //si el caracter recibido es el comando p
	   /*Guardo como cadenas la posición de ambos motores*/
         String alt = String (posicion_alt); 
         String az = String (posicion_az);
         String pos = az + "°\n" + alt + "°\n"; //Las uno en una única
         Serial.print("\nPosicion servos:\n"); 
         Serial.println(pos);			 //y la envío
         client.print(pos);
         client.flush();
       }
       else if (c == 'P') { //Si es el comando P
         Serial.print("\nPosicion satelite:");
         i = 0;
	/*Leo, guardándolos en el vector data, los 12 caracteres de datos
	que debo tomar para obtener las coordenadas a cambiar*/
         while (i < 12) {
           data[i] = (char)client.read();
           Serial.print(data[i]);
           i++;
         }
         Serial.print("\n");
         Serial.println("---");

	 /*Guardo como string el vector data y lo divido en substrings 
	 para separar los datos de azimut de los de altitud*/

         data_str = data;  
         data_az = data_str.substring(1, 6); 
         posicion_az = data_az.toInt(); //Guardo el dato como un entero
         data_alt = data_str.substring(7, 12);
         posicion_alt = data_alt.toInt(); //Guardo el dato como un entero

	 /*Si la coordenada supera el límite de los 180 del servo azimut, 
	 invierto la coordenada para alcanzar el grado desde el 0, e 
	 invierto de igual manera para el servo altitud, obteniendo el grado
	 de elevacion respecto de los 180*/

	   if(posicion_az>180){
	   posicion_az = posicion_az - 180;
	   posicion_alt = 180 - posicion_alt;}
           altitud.write(posicion_alt);
           azimut.write(posicion_az);
           client.print("RPRT 0");    //Lectura correcta
       }
       else if (c == 'q') {
         client.flush();
         client.stop();
       }
       else if (c == 'S') {
         client.flush();
         client.stop();
       }
      
     }
   }
   
   delay(1);

   // cierro la conexión:
   client.stop();
   Serial.println("client disconnected");
 }
}
