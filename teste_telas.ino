#include <LiquidCrystal.h>

#define TELA_INICIO 0
#define TELA_MENU1 1
#define TELA_MENU2 2
#define PORTA_UP 7
#define PORTA_DOWN 6
#define PORTA_ENTER 5
#define PORTA_BACK 4
#define INDICE_UP 0
#define INDICE_DOWN 1
#define INDICE_ENTER 2
#define INDICE_BACK 3

LiquidCrystal lcd(2, 3, 11, 10, 9, 8);

byte tela_atual = TELA_INICIO;
byte botao_porta[4] = {PORTA_UP, PORTA_DOWN, PORTA_ENTER, PORTA_BACK};
byte botao_estado[4] = {0};
byte posicao_seta = 1;

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
  Serial.begin(9600);
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
    	tela_menu(1);
     	break;
  }
}
void tela_inicio(void)
{  
  lcd.setCursor(0, 0);
  lcd.print(texto_inicio[0]);
  lcd.setCursor(0, 1);
  lcd.print(texto_inicio[1]);
 
  if (digitalRead(botao_porta[INDICE_ENTER]) == HIGH)
    tela_atual = TELA_MENU1;
}
void tela_menu(byte tela)
{
  if (tela == 1)
  {
    lcd.setCursor(2, 0);
    lcd.print("Ver sensores  ");
    lcd.setCursor(2, 1);
    lcd.print("Ver potenciom ");
  }
  if (tela == 2)
  {
    lcd.setCursor(2, 0);
    lcd.print("Historico     ");
    lcd.setCursor(2, 1);
    lcd.print("Configuracoes ");
  }
  
  lcd.setCursor(0, posicao_seta);
  lcd.print("> ");
  lcd.setCursor(0, !posicao_seta);
  lcd.print("  ");
}
