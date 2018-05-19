/*
  Este arquivo é apenas um teste para a
  criacao do menu interativo do SHIU
*/

#include <LiquidCrystal.h>

#define UP 1        //  
#define DOWN 2      //NUMEROS
#define ENTER 3     //ARBITRARIOS
#define BACK 4      //

int botao_porta[4] = {UP, DOWN, ENTER, BACK};
bool botao_estado[4];   // informa se um botao esta ou nao ligado

LiquidCrystal lcd = {5, 6, 7, 8, 9, 10} // numeros arbitrarios


void setup()
{
  lcd.begin();
  lcd.nocursor();   // nao eh necessario mostrar o cursor na tela

  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(ENTER, INPUT);
  pinMode(BACK, INPUT);

  for (int i = 0; i < 4; i++)
    botao_estado[i] = digitalRead(botao_porta[i]);

  lcd.setcursor(0, 0);
  lcd.print("Welcome to shiu");
  lcd.setcursor(3, 1);
  lcd.print("Press ENTER");

  while (botao_estado[ENTER] == LOW)    // o programa so vai iniciar
    ;                                   // quando apertar ENTER
}

void loop()
{
}

