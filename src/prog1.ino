#include <DHT.h>

#define PIN_BTN_N 23
#define PIN_BTN_P 22
#define PIN_BTN_K 21

#define PIN_DHT 13
#define DHTTYPE DHT22

#define PIN_LDR 15
#define PIN_RELAY 2

#define GerarCSV true

DHT dht(PIN_DHT, DHTTYPE);

int valorLDR = 0;
float phSimulado = 0.0;
float umidadeSimulada = 0.0;

bool estadoN = false;
bool estadoP = false;
bool estadoK = false;
bool bombaLigada = false;

bool vaiChover = false; 

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BTN_N, INPUT_PULLUP);
  pinMode(PIN_BTN_P, INPUT_PULLUP);
  pinMode(PIN_BTN_K, INPUT_PULLUP);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH);
  dht.begin();

  umidadeSimulada = dht.readHumidity();

  if (GerarCSV) {
    Serial.println("tempo,umidadeSimulada,phSimulado,estadoN,estadoP,estadoK,bomba,vaiChover");
  }

  Serial.println("Digite 'S' para indicar que vai chover ou 'N' para indicar que não vai chover:");
}

void loop() {
  // Leitura de entrada do usuário via Serial
  if (Serial.available() > 0) {
    char entrada = Serial.read();
    entrada = toupper(entrada);
    if (entrada == 'S') {
      vaiChover = true;
      Serial.println("Previsão de chuva detectada!");
    } else if (entrada == 'N') {
      vaiChover = false;
      Serial.println("Sem previsão de chuva.");
    }
  }

  // Leitura dos botões e sensores
  estadoN = digitalRead(PIN_BTN_N) == LOW;
  estadoP = digitalRead(PIN_BTN_P) == LOW;
  estadoK = digitalRead(PIN_BTN_K) == LOW;

  valorLDR = analogRead(PIN_LDR);
  phSimulado = map(valorLDR, 0, 4095, 0, 14);

  if (estadoN) phSimulado += 5.0;
  if (estadoP) phSimulado += 3.5;
  if (estadoK) phSimulado -= 2.5;
  phSimulado = constrain(phSimulado, 0, 14);

  // Define limites dinâmicos de irrigação conforme previsão
  float limiteInferior = vaiChover ? 40.0 : 60.0;
  float limiteSuperior = vaiChover ? 60.0 : 80.0;

  // Controle de irrigação com base nos limites
  if (umidadeSimulada < limiteInferior) {
    digitalWrite(PIN_RELAY, LOW);
    bombaLigada = true;
  } else if (umidadeSimulada >= limiteSuperior) {
    digitalWrite(PIN_RELAY, HIGH);
    bombaLigada = false;
  }

  // Simulação da variação de umidade
  if (bombaLigada) {
    umidadeSimulada += 5.0;
  } else {
    umidadeSimulada -= 1.0;
  }

  umidadeSimulada = constrain(umidadeSimulada, 0, 100);

  // Saída de dados
  if (GerarCSV) {
    Serial.print(millis() / 1000);
    Serial.print(",");
    Serial.print(umidadeSimulada, 2);
    Serial.print(",");
    Serial.print(phSimulado, 2);
    Serial.print(",");
    Serial.print(estadoN);
    Serial.print(",");
    Serial.print(estadoP);
    Serial.print(",");
    Serial.print(estadoK);
    Serial.print(",");
    Serial.print(bombaLigada);
    Serial.print(",");
    Serial.println(vaiChover);
    delay(10);
  } else {
    Serial.print("Botões: N=");
    Serial.print(estadoN);
    Serial.print(" P=");
    Serial.print(estadoP);
    Serial.print(" K=");
    Serial.println(estadoK);

    Serial.print("pH simulado: ");
    Serial.println(phSimulado);
    if (phSimulado < 6.0 || phSimulado > 6.8) {
      Serial.println("⚠️ ALERTA: pH fora do intervalo ideal para tomate!");
    }

    Serial.print("Umidade simulada: ");
    Serial.println(umidadeSimulada);

    Serial.print("Previsão de chuva: ");
    Serial.println(vaiChover ? "SIM" : "NÃO");

    Serial.print("Limite inferior (ligar bomba): ");
    Serial.println(limiteInferior);
    Serial.print("Limite superior (desligar bomba): ");
    Serial.println(limiteSuperior);

    Serial.print("Bomba: ");
    Serial.println(bombaLigada ? "LIGADA" : "DESLIGADA");

    Serial.println("--------------------------");
    delay(2000);
  }
}
