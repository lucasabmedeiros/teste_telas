#include <LiquidCrystal.h>
#include <EEPROM.h>

#define NUM_SENSOR 		  4
#define NIVEL_LIMITE 	 75
#define DELAY_BOTAO 	50

#define TELA_INICIO 0
#define TELA_MENU1 1
#define TELA_MENU2 2
#define TELA_VER_SENSOR 3
#define TELA_VER_POT 4
#define TELA_HISTORICO 5
#define TELA_CONFIG1 6
#define TELA_CONFIG2 7
#define TELA_CONFIG3 8
#define TELA_SENSIBILIDADE 9
#define TELA_SENSOR_ON_OFF 10
#define TELA_MODO_VARIACAO 11
#define TELA_POTENC_IDEAL 12
#define TELA_RESETAR_CONFIG 13


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

void escolher_tela(void);
void tela_inicio(void);
void tela_menu(byte tela);
void tela_ver_sensor(void);
void tela_ver_pot(void);
void tela_config(int tela);
void mostrar_seta(void);
void tela_sensibilidade(void);
void mod_tolerancia(char c);
void tela_potenc_ideal(void);
void mod_pot_ideal(int pot, char botao);
void tela_resetar_config(void);
void conf_padrao(void);
void tela_ligar_sensor(void);
void mod_chave(int sensor, bool status);
void tela_modo_variacao(void);
void tela_historico(void);
bool ler_botao(int indice);
void limpa(byte linha);

LiquidCrystal lcd(2, 3, 11, 10, 9, 8);

int  sensor_sinal[NUM_SENSOR]         = {10, 20, 30, 40};
int  potenciometro_sinal[NUM_SENSOR]  = {100, 200, 300, 400};
int  media_total                      = 25;

byte tela_atual = TELA_INICIO;
byte botao_porta[4] = {PORTA_UP, PORTA_DOWN, PORTA_ENTER, PORTA_BACK};
byte botao_estado[4] = {0};
int  posicao_escolha = 0;
bool limpar_linha = true;


String texto_inicio[2] = 
{
  "Bemvindo ao SHIU",
  "  Aperte ENTER  "
};
String texto_menu[4]= 
{
  " Ver sensores   ",
  " Ver potenciom  ",
  " Historico      ",
  " Configuracoes  "
};
String texto_config[6] = 
{
  " Sensibilidade  ",
  " Sensor on-off  ",
  " Modo variacao  ",
  " Potenc ideal   ",
  " Resetar config ",
  "                "
};
String texto_sensibilidade[2] = 
{
  "Atual ",
  "Aperte UP-DOWN  "
};
String texto_potenc_ideal[2] = 
{
  " Pt1 Pt2 Pt3 Pt4",
  "                "
};
String texto_ligar_sensor[2] = 
{
  " Sn1 Sn2 Sn3 Sn4",
  "                "
};
String texto_resetar_config[2] = 
{
  "  Tem certeza?  ",
  "   Enter Back   "
};
String texto_modo_variacao[2] = 
{
  " Constante      ",
  " Hora do dia    "
};
String texto_historico[2] = 
{
  "Qntd de sirenes ",
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
  
  ep.tolerancia = 50;
  ep.contador = 3;
  
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
    case TELA_SENSIBILIDADE:
    	tela_sensibilidade();
    	break;
    case TELA_POTENC_IDEAL:
    	tela_potenc_ideal();
    	break;
	case TELA_RESETAR_CONFIG:
    	tela_resetar_config();
    	break;
    case TELA_SENSOR_ON_OFF:
    	tela_ligar_sensor();
    	break;
    case TELA_MODO_VARIACAO:
    	tela_modo_variacao();
    	break;
    case TELA_HISTORICO:
    	tela_historico();
    	break;
    
  }
}
void tela_inicio(void)
{
  lcd.setCursor(0, 0);
  lcd.print(texto_inicio[0]);
  lcd.setCursor(0, 1);
  lcd.print(texto_inicio[1]);
 
  if (ler_botao(INDICE_ENTER))
    tela_atual = TELA_MENU1;
}
void tela_menu(byte tela)
{
  mostrar_seta();
  
  if (tela == 1)
  {
    lcd.setCursor(1, 0);
    lcd.print(texto_menu[0]);
    lcd.setCursor(1, 1);
    lcd.print(texto_menu[1]);
  }
  if (tela == 2)
  {
    lcd.setCursor(1, 0);
    lcd.print(texto_menu[2]);
    lcd.setCursor(1, 1);
    lcd.print(texto_menu[3]);
  }

  if (ler_botao(INDICE_UP))
  {
    posicao_escolha--;
      
    if (posicao_escolha == 1)
      	tela_atual = TELA_MENU1;
    
    if (posicao_escolha == -1)
      posicao_escolha = 0;
  }
  else if (ler_botao(INDICE_DOWN))
  {
    posicao_escolha++;
      
    if (posicao_escolha == 2)
      	tela_atual = TELA_MENU2;
    
    if (posicao_escolha == 4)
      posicao_escolha = 3;
  }
  
  
  if (ler_botao(INDICE_ENTER))
  {
    switch (posicao_escolha)
    {
      case 0:
      	tela_atual = TELA_VER_SENSOR;
      	break;
      case 1:
        tela_atual = TELA_VER_POT;
      	break;
      case 2:
    	tela_atual = TELA_HISTORICO;
      	break;
      case 3:
      	tela_atual = TELA_CONFIG1;
      	break;
    }
    posicao_escolha = 0;
  }
  
  if (ler_botao(INDICE_BACK))
  {
    posicao_escolha = 0;
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
      lcd.print("off ");
  }
  
  limpa(1);
  
  lcd.setCursor(0, 1);
  lcd.print("Media ");
  lcd.setCursor(6, 1);
  lcd.print(media_total);
  
  if (ler_botao(INDICE_BACK))
  {
    tela_atual = TELA_MENU1;
    limpar_linha = true;
  }
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
  
  if (ler_botao(INDICE_BACK))
  	tela_atual = TELA_MENU1;
}
void tela_config(int tela)
{
  mostrar_seta();
  switch (tela)
  {
    case 1:
    	lcd.setCursor(1, 0);
    	lcd.print(texto_config[0]);
    	lcd.setCursor(1, 1);
    	lcd.print(texto_config[1]);
    	break;
    case 2:
    	lcd.setCursor(1, 0);
    	lcd.print(texto_config[2]);
    	lcd.setCursor(1, 1);
    	lcd.print(texto_config[3]);
    	break;
    case 3:
    	lcd.setCursor(1, 0);
    	lcd.print(texto_config[4]);
    	lcd.setCursor(1, 1);
    	lcd.print(texto_config[5]);
    	break;
  }
  
  if (ler_botao(INDICE_UP))
  {
    posicao_escolha--;
    
    if (posicao_escolha == 1)
      tela_atual = TELA_CONFIG1;
    if (posicao_escolha == 3)
      tela_atual = TELA_CONFIG2;
    
    if (posicao_escolha == -1)
		posicao_escolha = 0; 
  }
  else if (ler_botao(INDICE_DOWN))
  {
    posicao_escolha++;
    
    if (posicao_escolha == 2)
      tela_atual = TELA_CONFIG2;
    if (posicao_escolha == 4)
      tela_atual = TELA_CONFIG3;
    
    if (posicao_escolha == 5)
      posicao_escolha = 4;
  }
  
  else if (ler_botao(INDICE_ENTER))
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
    
  }
  else if (ler_botao(INDICE_BACK))
  {
    tela_atual = TELA_MENU1;
    posicao_escolha = 0;
  }
}
void tela_sensibilidade(void)
{
  bool executa = false;
  char alteracao;
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  limpa(0);
  
  lcd.setCursor(0, 0);
  lcd.print(texto_sensibilidade[0]);
  lcd.setCursor(6, 0);
  lcd.print(ep.tolerancia);
  lcd.setCursor(0, 1);
  lcd.print(texto_sensibilidade[1]);
  
  if (ler_botao(INDICE_UP))
  {
    alteracao = '+';
    executa = true;
  }
  else if (ler_botao(INDICE_DOWN))
  {
    alteracao = '-';
    executa = true;
  }
  
  if (executa)
  {
  	mod_tolerancia(alteracao);
    executa = false;
  }
  
  if (ler_botao(INDICE_BACK))
  {
    tela_atual = TELA_CONFIG1;
    limpar_linha = true;
  }
}
void mod_tolerancia(char c) //modificar_tolerancia [funcao auxiliar eeprom]
{
  t_eeprom ep;

  EEPROM.get(0, ep);

  if(c=='+')
    ep.tolerancia = ep.tolerancia + 1;
  else if(c=='-')  
    ep.tolerancia = ep.tolerancia - 1;
  
  EEPROM.put(0, ep);
  
}
void tela_potenc_ideal(void)
{
  static char *seta = ">";
  static bool iniciar_alteracao = false;
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  lcd.setCursor(0, 0);
  lcd.print(texto_potenc_ideal[0]);
  
  limpa(1);
  
  lcd.setCursor(4 * posicao_escolha, 0);
  lcd.print(seta);
  for (int i = 0; i < NUM_SENSOR;  i++)
  {
    lcd.setCursor(4 * i + 1, 1);
    lcd.print(ep.potenciometro_ideal[i]);
  }
  
  if (!iniciar_alteracao)
  {
  	if (ler_botao(INDICE_UP) && posicao_escolha < 3)
    	posicao_escolha++;
  	if (ler_botao(INDICE_DOWN) && posicao_escolha > 0) 
    	posicao_escolha--;
  }
  
  if (ler_botao(INDICE_ENTER))
  {
    seta = "#";
    iniciar_alteracao = true;
  }
  
  
  if (iniciar_alteracao)
  {
    if (ler_botao(INDICE_UP))
      	mod_pot_ideal(posicao_escolha, '+');
    else if (ler_botao(INDICE_DOWN))
      	mod_pot_ideal(posicao_escolha, '-');
      
    else if (ler_botao(INDICE_BACK))
    {
      seta = ">";
      iniciar_alteracao = false;
      posicao_escolha = 0;
    }
  }
  else if (ler_botao(INDICE_BACK))
  {
   	tela_atual = TELA_CONFIG1;
   	limpar_linha = true;
  }
}
void mod_pot_ideal(int pot, char botao) //modificar_potenciometro_ideal [funcao auxiliar eeprom]
{
  t_eeprom ep;

  EEPROM.get(0, ep);

  if(botao=='+')
    ep.potenciometro_ideal[pot] = ep.potenciometro_ideal[pot] + 1;
  else if(botao=='-')
    ep.potenciometro_ideal[pot] = ep.potenciometro_ideal[pot] - 1;

  EEPROM.put(0, ep);
}
void tela_resetar_config(void)
{
  lcd.setCursor(0, 0);
  lcd.print(texto_resetar_config[0]);
  lcd.setCursor(0, 1);
  lcd.print(texto_resetar_config[1]);
  
  if (ler_botao(INDICE_ENTER))
  {
    conf_padrao();
    tela_atual = TELA_INICIO;
  }
  else if (ler_botao(INDICE_BACK))
    tela_atual = TELA_CONFIG1;
}
void conf_padrao(void)//carrega a parte inicial do eeprom(endereco 0) com a struct das informacoes do eeprom
{                     //modifica todas as configuracoes para "configuracoes de fabrica"
  t_eeprom ep;

  ep.contador=0;
  ep.tolerancia = NIVEL_LIMITE;

  for(int i=0; i<NUM_SENSOR; i++)
  {
    ep.sensor_chave[i]=true;
    ep.potenciometro_ideal[i] = 540;
  }

  EEPROM.put(0, ep);
}
void tela_ligar_sensor(void)
{
  static char *icone = ">";
  static bool iniciar_alteracao = false;
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  lcd.setCursor(0, 0);
  lcd.print(texto_ligar_sensor[0]);
  
  limpa(1);
  
  lcd.setCursor(4 * posicao_escolha, 0);
  lcd.print(icone);
  for (int i = 0; i < NUM_SENSOR;  i++)
  {
    lcd.setCursor(4 * i + 1, 1);
    if (ep.sensor_chave[i])
    	lcd.print(" on");
    else
      	lcd.print("off");
  }
  
  if (!iniciar_alteracao)
  {
  	if (ler_botao(INDICE_UP) && posicao_escolha < 3)
    	posicao_escolha++;
  	if (ler_botao(INDICE_DOWN) && posicao_escolha > 0) 
    	posicao_escolha--;
  }
  
  if (ler_botao(INDICE_ENTER))
  {
    icone = "#";
    iniciar_alteracao = true;
  }
  
  
  if (iniciar_alteracao)
  {
    if (ler_botao(INDICE_UP))
		mod_chave(posicao_escolha, true);
    else if (ler_botao(INDICE_DOWN))
		mod_chave(posicao_escolha, false);
      
    if (ler_botao(INDICE_BACK))
    {
      icone = ">";
      iniciar_alteracao = false;
      posicao_escolha = 0;
    }
  }
  else if (ler_botao(INDICE_BACK))
  {
   	tela_atual = TELA_CONFIG1;
   	limpar_linha = true;
  }
}
void mod_chave(int sensor, bool status) //modificar_chave - do sensor [funcao auxiliar eeprom]
{
  t_eeprom ep;

  EEPROM.get(0, ep);

  ep.sensor_chave[sensor] = status;

  EEPROM.put(0, ep);
}
void tela_modo_variacao(void)
{
  lcd.setCursor(1, 0);
  lcd.print(texto_modo_variacao[0]);
  lcd.setCursor(1, 1);
  lcd.print(texto_modo_variacao[1]);
 
  mostrar_seta();
  
  /* 
  	Ambos os modos ainda nao estao definidos
  */
  
  if (ler_botao(INDICE_UP) || ler_botao(INDICE_DOWN))
    posicao_escolha = !posicao_escolha;
    
  if (ler_botao(INDICE_BACK))
    tela_atual = TELA_CONFIG1;
}
void tela_historico(void)
{
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  lcd.setCursor(0, 0);
  lcd.print(texto_historico[0]);
  
  limpa(1);
  
  lcd.setCursor(0, 1);
  lcd.print(ep.contador);
  
  if (ler_botao(INDICE_BACK))
  {
    tela_atual = TELA_MENU1;
    limpar_linha = true;
  }
}

bool ler_botao(int indice)
{
  static bool estado1, estado2;
  static unsigned long tempo = millis();
  
  estado1 = digitalRead(botao_porta[indice]);
  
  if (millis() - tempo > DELAY_BOTAO)
  {
  	estado2 = digitalRead(botao_porta[indice]);
    
    if (estado1 == estado2 && estado1)
    {
      tempo = millis();
      return true;
    }
  }
  return false;
}
void mostrar_seta(void)
{
  lcd.setCursor(0, posicao_escolha%2);
  lcd.print("> ");
  lcd.setCursor(0, !(posicao_escolha%2));
  lcd.print("  ");
}
void limpa(byte linha)
{
  if (limpar_linha)
  {
  	lcd.setCursor(0, linha);
  	lcd.print("                ");
    limpar_linha = false;
  }
}
