#include <Keypad.h>
#include <Servo.h>

// Configuración de los teclados 4x4
const byte FILAS = 4;
const byte COLUMNAS = 4;
char keys[FILAS][COLUMNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinesFilas1[FILAS] = {28, 29, 30, 31};
byte pinesColumnas1[COLUMNAS] = {32, 33, 34, 35};
Keypad teclado1 = Keypad(makeKeymap(keys), pinesFilas1, pinesColumnas1, FILAS, COLUMNAS);

byte pinesFilas2[FILAS] = {42, 43, 44, 45};
byte pinesColumnas2[COLUMNAS] = {46, 47, 48, 49};
Keypad teclado2 = Keypad(makeKeymap(keys), pinesFilas2, pinesColumnas2, FILAS, COLUMNAS);

// Configuración de LEDs
int LED_VERDE1 = 22;
int LED_ROJO1 = 23;
int LED_VERDE2 = 24;
int LED_ROJO2 = 25;
int LUZ = 5;

// Configuración del servomotor
Servo servo;

// Configuración del buzzer
int BUZZER = 4;

// Configuración del sensor Booster
const int trigPin = 8;
const int echoPin = 7;

// Variables para claves de acceso
char TECLA1, TECLA2;
char CLAVE1[7], CLAVE2[7];
char CLAVES_VALIDAS1[3][7] = {"123456", "000000", "111222"};
char CLAVES_VALIDAS2[3][7] = {"654321", "111111", "222222"};
byte INDICE1 = 0, INDICE2 = 0;

void setup() {
  Serial.begin(9600);

  // Configuración de pines
  pinMode(LED_VERDE1, OUTPUT);
  pinMode(LED_ROJO1, OUTPUT);
  pinMode(LED_VERDE2, OUTPUT);
  pinMode(LED_ROJO2, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LUZ, OUTPUT);

  servo.attach(3);
  servo.write(0);

  // Inicialización de LEDs
  digitalWrite(LED_ROJO1, HIGH);
  digitalWrite(LED_VERDE1, LOW);
  digitalWrite(LED_ROJO2, HIGH);
  digitalWrite(LED_VERDE2, LOW);
  digitalWrite(LUZ, HIGH);
}

void loop() {
  // Leer teclados
  TECLA1 = teclado1.getKey();
  TECLA2 = teclado2.getKey();

  if (TECLA1) manejarTeclado(TECLA1, CLAVE1, CLAVES_VALIDAS1, INDICE1, LED_VERDE1, LED_ROJO1, true);
  if (TECLA2) manejarTeclado(TECLA2, CLAVE2, CLAVES_VALIDAS2, INDICE2, LED_VERDE2, LED_ROJO2, false);

  // Asegurar que el buzzer esté apagado en cada ciclo de loop
  digitalWrite(BUZZER, LOW);  // Apaga el zumbador

}

void manejarTeclado(char tecla, char *clave, char claves_validas[3][7], byte &indice, int led_verde, int led_rojo, bool activar_servo) {
  clave[indice] = tecla;
  indice++;
  Serial.print(tecla);

  if (indice == 6) { // Cuando se ha ingresado una clave completa
    bool clave_correcta = false;
    for (int i = 0; i < 3; i++) {
      if (!strcmp(clave, claves_validas[i])) {
        clave_correcta = true;
        break;
      }
    }

    if (clave_correcta) {
      Serial.println(" Correcta");
      digitalWrite(led_verde, HIGH);
      digitalWrite(led_rojo, LOW);

      // Sonido de clave correcta
      tone(BUZZER, 1500, 500); // Tono agudo y corto
      delay(500);
      noTone(BUZZER);

      if (activar_servo) {
        servo.write(70);
        delay(3000);
        servo.write(0);
      }
    } else {
      Serial.println(" Incorrecta");
      digitalWrite(led_rojo, HIGH);
      digitalWrite(led_verde, LOW);

      // Sonido de clave incorrecta
      tone(BUZZER, 500, 500); // Tono grave y más largo
      delay(500);
      noTone(BUZZER);
    }

    // Reiniciar LEDs y buzzer
    digitalWrite(led_verde, LOW);
    digitalWrite(led_rojo, LOW);
    noTone(BUZZER);

    // Reiniciar entrada
    indice = 0;
    memset(clave, 0, 7);
  }
}

