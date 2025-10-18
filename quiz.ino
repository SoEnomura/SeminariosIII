#include <LiquidCrystal.h>

// === Definição dos pinos ===
int backLight = 13;
int LED_VERDE = A0;
int LED_VERMELHO = A1;
const int BOTAO_GRAVE = 6;
const int BOTAO_AGUDO = 7;
const int BUZZER = 8;
const int MOTOR = 9;
const int BTN_RESET = A0;

// === Pinos do display LCD ===
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

// === Parâmetros do jogo ===
int notaAtual; // 0 = grave, 1 = aguda
int pontos = 0;
unsigned long tempoResposta;
bool aguardandoResposta = false;

// === Frequências das notas ===
const int FREQ_GRAVE = 200;
const int FREQ_AGUDA = 800;

// === Intensidade do motor vibratório ===
const int VIB_GRAVE = 200;
const int VIB_AGUDA = 100;

// === Função de inicialização ===
void setup() {
  pinMode(backLight, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BOTAO_GRAVE, INPUT);
  pinMode(BOTAO_AGUDO, INPUT);
  pinMode(BTN_RESET, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MOTOR, OUTPUT);

  Serial.begin(9600);
  randomSeed(analogRead(0));

  digitalWrite(backLight, HIGH);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Quiz Musical");
  lcd.setCursor(0, 1);
  lcd.print("Inclusivo");
  delay(2000);
  lcd.clear();
  lcd.print("Pontos: 0");
  delay(1000);
}

// === Função principal ===
void loop() {
  // Verifica botão de reset
  if (digitalRead(BTN_RESET) == HIGH) {
    delay(50);
    if (digitalRead(BTN_RESET) == HIGH) {
      while (digitalRead(BTN_RESET) == HIGH); // espera botão ser solto
      pontos = 0;
      lcd.clear();
      lcd.print("Pontuacao zerada");
      delay(1500);
      lcd.clear();
      lcd.print("Pontos: 0");
      delay(1000);
      aguardandoResposta = false;
    }
  }

  if (!aguardandoResposta) {
    iniciarRodada();
  } else {
    if (millis() - tempoResposta > 10000) {
      // Tempo esgotado
      lcd.clear();
      lcd.print("Tempo esgotado!");
      delay(2000);
      lcd.clear();
      lcd.print("Pontos: ");
      lcd.print(pontos);
      aguardandoResposta = false;
      delay(1000);
    } else {
      verificarBotoes();
    }
  }
}

// === Inicia uma nova rodada ===
void iniciarRodada() {
  notaAtual = random(0, 2);
  if (notaAtual == 0) {
    tone(BUZZER, FREQ_GRAVE, 500);
    analogWrite(MOTOR, VIB_GRAVE);
    delay(500);
    analogWrite(MOTOR, 0);
  } else {
    tone(BUZZER, FREQ_AGUDA, 500);
    analogWrite(MOTOR, VIB_AGUDA);
    delay(500);
    analogWrite(MOTOR, 0);
  }
  lcd.clear();
  lcd.print("Responda...");
  tempoResposta = millis();
  aguardandoResposta = true;
}

// === Verifica se algum botão foi pressionado ===
void verificarBotoes() {
  if (digitalRead(BOTAO_GRAVE) == HIGH) {
    verificarResposta(0);
    aguardandoResposta = false;
    delay(300);
  }
  if (digitalRead(BOTAO_AGUDO) == HIGH) {
    verificarResposta(1);
    aguardandoResposta = false;
    delay(300);
  }
}

// === Verifica se a resposta está correta ===
void verificarResposta(int resposta) {
  lcd.clear();
  if (resposta == notaAtual) {
    pontos++;
    lcd.print("Acertou!");
    lcd.setCursor(0, 1);
    lcd.print("Pontos: ");
    lcd.print(pontos);
    tone(BUZZER, 600, 300);
    digitalWrite(LED_VERDE, HIGH);
    delay(1500);
    digitalWrite(LED_VERDE, LOW);
  } else {
    lcd.print("Errou!");
    lcd.setCursor(0, 1);
    lcd.print("Pontos: ");
    lcd.print(pontos);
    tone(BUZZER, 100, 300);
    digitalWrite(LED_VERMELHO, HIGH);
    delay(1500);
    digitalWrite(LED_VERMELHO, LOW);
  }
  lcd.clear();
  lcd.print("Pontos: ");
  lcd.print(pontos);
}
