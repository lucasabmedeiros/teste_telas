#include <LiquidCrystal.h>

#define TELA_INICIO 0
#define TELA_MENU1 1
#define TELA_MENU2 2
#define UP 7
#define DOWN 6
#define ENTER 5
#define BACK 4

LiquidCrystal lcd(2, 3, 11, 10, 9, 8);

byte tela_atual = TELA_INICIO;
byte botao_porta[4] = {UP, DOWN, ENTER, BACK};
byte botao_estado[4] = {0};

String texto_inicio[2] = 
{
  "Bemvindo ao SHIU",
  "  Aperte ENTER  "
};
String texto_menu[4]= {
  "  Ver sensores  ",
  "  Ver potenciom ",
  "  Historico     ",
  "  Configuracoes "
};

void setup()
{
  lcd.begin(16,2);
  lcd.noCursor();
  
  pinMode(13, OUTPUT);
}

void loop()
{
  escolher_tela();
}
void escolher_tela(void)
{
  switch(tela_atual)
  {
    case TELA_INICIO:
    	tela_inicio();
    	break;
    case TELA_MENU1:
    	lcd.clear();
     	break;
  }
}
bool ler_botao(byte sinal)
{
  byte tempo, atraso_debounce = 5;

  if (digitalRead(botao_porta[sinal]) == HIGH)
    tempo = millis();

  if ((millis() - tempo) > atraso_debounce)
    if (digitalRead(botao_porta[sinal]) == HIGH)
		return true;
    
  return false;
}
void tela_inicio(void)
{
  lcd.setCursor(0, 0);
  lcd.print(texto_inicio[0]);
  lcd.setCursor(0, 1);
  lcd.print(texto_inicio[1]);
  
  if (ler_botao(botao_porta[ENTER]))
  {
    digitalWrite(13, HIGH);
    tela_atual = TELA_MENU1;
  }
}
