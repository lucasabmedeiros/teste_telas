#include <LiquidCrystal.h>

#define NUM_SENSOR 4

#define TELA_INICIO 0
#define TELA_MENU1 1
#define TELA_MENU2 2
#define TELA_VER_SENSOR 3

#define PORTA_UP 7
#define PORTA_DOWN 6
#define PORTA_ENTER 5
#define PORTA_BACK 4
#define INDICE_UP 0
#define INDICE_DOWN 1
#define INDICE_ENTER 2
#define INDICE_BACK 3

typedef struct st_eeprom{
  int contador;
  int tolerancia;
  bool sensor_chave[NUM_SENSOR];        
  short potenciometro_ideal[NUM_SENSOR];
}t_eeprom;

LiquidCrystal lcd(2, 3, 11, 10, 9, 8);

int  sensor_sinal[NUM_SENSOR]         = {10, 20, 30, 40};
int  media_total                      = 25;

byte tela_atual = TELA_INICIO;
byte botao_porta[4] = {PORTA_UP, PORTA_DOWN, PORTA_ENTER, PORTA_BACK};
byte botao_estado[4] = {0};
byte posicao_seta = 0;
byte posicao_escolha = 0;

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
    case TELA_MENU2:
    	tela_menu(2);
     	break;
    case TELA_VER_SENSOR:
    	tela_ver_sensor();
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
    lcd.print(texto_menu[0]);
    lcd.setCursor(2, 1);
    lcd.print(texto_menu[1]);
  }
  if (tela == 2)
  {
    lcd.setCursor(2, 0);
    lcd.print(texto_menu[2]);
    lcd.setCursor(2, 1);
    lcd.print(texto_menu[3]);
  }
  
  lcd.setCursor(0, posicao_seta);
  lcd.print("> ");
  lcd.setCursor(0, !posicao_seta);
  lcd.print("  ");
  
  if (digitalRead(botao_porta[INDICE_UP]))
  {
    posicao_seta = !posicao_seta;
    posicao_escolha--;
    
    if (posicao_escolha == -1)
    {
      tela_atual = TELA_MENU2;
      posicao_escolha = 3;
    }
    else if (posicao_escolha == 2)
      tela_atual = TELA_MENU1;
  }
  else if (digitalRead(botao_porta[INDICE_DOWN]))
  {
    posicao_seta = !posicao_seta;
    posicao_escolha++;
    
    if (posicao_escolha == 4)
    {
      tela_atual = TELA_MENU1;
      posicao_escolha = 0;
    }
    else if (posicao_escolha == 2)
      tela_atual = TELA_MENU2;
  }
  
  if (digitalRead(botao_porta[INDICE_ENTER]))
  {
    switch (posicao_escolha)
    {
      case 0:
      	tela_atual = TELA_VER_SENSOR;
      	tela_ver_sensor();
      	break;
      case 1:
      	break;
      case 2:
      	break;
      case 3:
      	break;
    }
  }
  
  if (digitalRead(botao_porta[INDICE_BACK]))
  {
    posicao_escolha = 0;
    posicao_seta = 0;
    tela_atual = TELA_INICIO;
  }
}
void tela_ver_sensor(void)
{
  t_eeprom ep;
  
  for (int i = 0; i < NUM_SENSOR; i++)
  {
    lcd.setCursor(4*i, 0);
    
    if (ep.sensor_chave[i])
    {
      lcd.print(sensor_sinal[i]);
      lcd.print("   ");
    }
    else
      lcd.print("XXX ");
  }
  lcd.setCursor(0, 1);
  lcd.print("Media           ");
  lcd.setCursor(6, 1);
  lcd.print(media_total);
}
