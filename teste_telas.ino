#include <LiquidCrystal.h>
#include <EEPROM.h>

#define NUM_SENSOR 4

#define TELA_INICIO 0
#define TELA_MENU1 1
#define TELA_MENU2 2
#define TELA_VER_SENSOR 3
#define TELA_VER_POT 4
#define TELA_CONFIG1 5
#define TELA_CONFIG2 6
#define TELA_CONFIG3 7
#define TELA_SENSIBILIDADE 8
#define TELA_SENSOR_ON_OFF 9
#define TELA_MODO_VARIACAO 10
#define TELA_POTENC_IDEAL 11
#define TELA_RESETAR_CONFIG 12

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

void tela_ver_sensor(void);
void tela_menu(byte tela);
void tela_inicio(void);
void escolher_tela(void);
void tela_ver_pot(void);

LiquidCrystal lcd(2, 3, 11, 10, 9, 8);

int  sensor_sinal[NUM_SENSOR]         = {10, 20, 30, 40};
int  potenciometro_sinal[NUM_SENSOR]  = {100, 200, 300, 400};
int  media_total                      = 25;

byte tela_atual = TELA_INICIO;
byte botao_porta[4] = {PORTA_UP, PORTA_DOWN, PORTA_ENTER, PORTA_BACK};
byte botao_estado[4] = {0};
byte posicao_seta = 0;
int  posicao_escolha = 0;


String texto_inicio[2] = 
{
  "Bemvindo ao SHIU",
  "  Aperte ENTER  "
};
String texto_menu[4]= 
{
  "  Ver sensores  ",
  "  Ver potenciom ",
  "  Historico     ",
  "  Configuracoes "
};
String texto_config[6] = 
{
  "  Sensibilidade ",
  "  Sensor on-off ",
  "  Modo variacao ",
  "  Potenc ideal  ",
  "  Resetar config",
  "                "
};


void setup()
{
  t_eeprom ep;
  
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.noCursor();
  
  for(int i=0; i<NUM_SENSOR; i++)
  {
    ep.sensor_chave[i]=true;
    ep.potenciometro_ideal[i] = 540;
  }
  
  EEPROM.put(0, ep);
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
     	break;
    case TELA_VER_POT:
    	tela_ver_pot();
     	break;
    case TELA_CONFIG1:
    	tela_config(1);
    	break;
    case TELA_CONFIG2:
    	tela_config(2);
    	break;
    case TELA_CONFIG3:
    	tela_config(3);
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
  
  Serial.print("Seta: ");
  Serial.println(posicao_seta);
  Serial.print("Escolha: ");
  Serial.println(posicao_escolha);
  Serial.println("");
  
  if (digitalRead(botao_porta[INDICE_UP]))
  {
    posicao_seta = !posicao_seta;
    posicao_escolha--;
      
    if (posicao_escolha == 1)
      	tela_atual = TELA_MENU1;
    
    if (posicao_escolha == -1)
    {
      posicao_seta = 0;
      posicao_escolha = 0;
    }
  }
  else if (digitalRead(botao_porta[INDICE_DOWN]))
  {
    posicao_seta = !posicao_seta;
    posicao_escolha++;
      
    if (posicao_escolha == 2)
      	tela_atual = TELA_MENU2;
    
    if (posicao_escolha == 4)
    {
      posicao_seta = 1;
      posicao_escolha = 3;
    }
  }
  
  
  if (digitalRead(botao_porta[INDICE_ENTER]))
  {
    switch (posicao_escolha)
    {
      case 0:
      	tela_atual = TELA_VER_SENSOR;
      	break;
      case 1:
        tela_atual = TELA_VER_POT;
      	break;
    //case 2:
    //  Por enquanto, nao ha historico a ser mostrado
      case 3:
      	tela_atual = TELA_CONFIG1;
      	break;
    }
    posicao_escolha = 0;
    posicao_seta = 0;    
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
  EEPROM.get(0, ep);
  
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
  
  if (digitalRead(botao_porta[INDICE_BACK]))
    tela_atual = TELA_MENU1;
}
void tela_ver_pot(void)
{
  t_eeprom ep;
  EEPROM.get(0, ep);
    
  for (int i = 0; i < 4; i++)
  {
    lcd.setCursor(4*i, 0);
    lcd.print(potenciometro_sinal[i]);
    lcd.print("  ");
  }
  lcd.setCursor(0, 1);
  lcd.print("Tol ");
  lcd.print(ep.tolerancia);
  lcd.print(" Ideal ");
  lcd.print(ep.potenciometro_ideal[0]);
  
  if (digitalRead(botao_porta[INDICE_BACK]))
  	tela_atual = TELA_MENU1;
}
void tela_config(int tela)
{
  switch (tela)
  {
    case 1:
    	lcd.setCursor(0, 0);
    	lcd.print(texto_config[0]);
    	lcd.setCursor(0, 1);
    	lcd.print(texto_config[1]);
    	break;
    case 2:
    	lcd.setCursor(0, 0);
    	lcd.print(texto_config[2]);
    	lcd.setCursor(0, 1);
    	lcd.print(texto_config[3]);
    	break;
    case 3:
    	lcd.setCursor(0, 0);
    	lcd.print(texto_config[4]);
    	lcd.setCursor(0, 1);
    	lcd.print(texto_config[5]);
    	break;
  }
  
  lcd.setCursor(0, posicao_seta);
  lcd.print("> ");
  lcd.setCursor(0, !posicao_seta);
  lcd.print("  ");
  
    posicao_seta = !posicao_seta;
    posicao_escolha++;
  
  if (digitalRead(botao_porta[INDICE_UP]))
  {
    posicao_seta = !posicao_seta;
    posicao_escolha--;
    
    if (posicao_escolha == -1)
    {
      posicao_seta = 0;
      posicao_escolha = 4;
      tela_atual = TELA_CONFIG3;
    }
    if (posicao_escolha == 1)
      tela_atual = TELA_CONFIG1;
    if (posicao_escolha == 3)
      tela_atual = TELA_CONFIG2;
  }
  else if (digitalRead(botao_porta[INDICE_DOWN]))
  {
    posicao_seta = !posicao_seta;
    posicao_escolha++;
    
    if (posicao_escolha == 2)
      tela_atual = TELA_CONFIG2;
    if (posicao_escolha == 4)
      tela_atual = TELA_CONFIG3;
    if (posicao_escolha == 5)
    {
      posicao_seta = 0;
      posicao_escolha = 0;
      tela_atual = TELA_CONFIG1;
    }
  }
  
  else if (digitalRead(botao_porta[INDICE_ENTER]))
  {
    switch (posicao_escolha)
    {
      case 0:
      	tela_atual = TELA_SENSIBILIDADE;
      	break;
      case 1:
      	tela_atual = TELA_SENSOR_ON_OFF;
      	break;
      case 2:
      	tela_atual = TELA_MODO_VARIACAO;
      	break;
      case 3:
      	tela_atual = TELA_POTENC_IDEAL;
      	break;
      case 4:
      	tela_atual = TELA_RESETAR_CONFIG;
      	break;
    }
    posicao_escolha = 0;
    posicao_seta = 0;
  }
  else if (digitalRead(botao_porta[INDICE_BACK]))
  {
    tela_atual = TELA_MENU1;
    posicao_escolha = 0;
    posicao_seta = 0;
  }
}
