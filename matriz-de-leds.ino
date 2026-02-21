// MATRIZ
int L[8] = {9, 12, 2, 11, 8, 3, 7, 4};      
int C[8] = {44, 6, 5, 10, A0, A1, A2, A3}; 

// JOYSTICK
int pinX = A9;
int pinY = A8;
int pinBotao = 52; 

// ESTADO 
bool tela[8][8] = {false}; 
int posX = 0, posY = 0;
unsigned long delayMov = 0;

unsigned long tempoBotaoPressionado = 0;
bool botaoEstavaPressionado = false;

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(L[i], OUTPUT);
    pinMode(C[i], OUTPUT);
    digitalWrite(L[i], LOW);
    digitalWrite(C[i], HIGH);
  }
  pinMode(pinBotao, INPUT_PULLUP);
}

void loop() {
  // MOVIMENTO DO JOYSTICK
  if (millis() - delayMov > 180) {
    int xVal = analogRead(pinX);
    int yVal = analogRead(pinY);

    if (xVal > 800) posX = min(7, posX + 1);
    else if (xVal < 200) posX = max(0, posX - 1);

    if (yVal > 800) posY = min(7, posY - 1); 
    else if (yVal < 200) posY = max(0, posY + 1);

    delayMov = millis();
  }

  // BOTÃO
  int leituraBotao = digitalRead(pinBotao);

  if (leituraBotao == LOW) { // Botão pressionado
    if (!botaoEstavaPressionado) {
      tempoBotaoPressionado = millis();
      botaoEstavaPressionado = true;
    }
    
    // Se segurar por mais de 2 segundos, limpa a matriz
    if (millis() - tempoBotaoPressionado > 2000) {
      limparTela();
      botaoEstavaPressionado = false; 
    }
  } 
  else { // Botão solto
    if (botaoEstavaPressionado) {
      // Se foi um clique rápido (menos de 2s), fixa o LED
      if (millis() - tempoBotaoPressionado < 2000) {
        if (tela[posY][posX]) {
          tela[posY][posX] = false;
        } else {
          tela[posY][posX] = true;
        }
      }
      botaoEstavaPressionado = false;
    }
  }

  // DESENHO DA MATRIZ
  desenharMatriz();
}

void limparTela() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      tela[i][j] = false;
    }
  }
  tela[posX][posY] = false;
  posX = 0;
  posY = 0;
  delay(200);
}

void desenharMatriz() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(L[i], HIGH); 
    for (int j = 0; j < 8; j++) {
      if ((i == posY && j == posX) || tela[i][j]) {
        digitalWrite(C[j], LOW);   
        delayMicroseconds(400);    
        digitalWrite(C[j], HIGH);  
      }
    }
    digitalWrite(L[i], LOW); 
  }
}