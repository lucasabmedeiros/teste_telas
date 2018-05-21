#include <LiquidCrystal.h>
#include <EEPROM.h>

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

#define UP 0
#define DOWN 1
#define ENTER 2
#define BACK 3
#define PORTA_UP 7
#define PORTA_DOWN 6
#define PORTA_ENTER 5
#define PORTA_BACK 4

typedef struct st_eeprom{
  int contador;
  int tolerancia;
  bool sensor_chave[NUM_SENSOR];        
  short potenciometro_ideal[NUM_SENSOR];
}t_eeprom;

/* -------------- Utilitarios ------------------- */
void mostrar_seta(void);
/*
	Mostra na tela o icone da seta 
    conforme a variavel posicao_escolha
*/
bool ler_botao(int indice);
/*
	Le o botao informado com debounce.
    Retorna verdadeiro caso a leitura seja HIGH,
    logo e preciso utilizar resitstores pulldown
*/
void limpa(byte linha);
/*
	Caso a flag limpar_linha seja verdadeira,
    substitui a linha indicada por espacos em branco
*/
/* ---------------------------------------------- */


/* ------------ Funcoes de tela ----------------- */
void LCD_menu_start(void);
/*
	Unica funcao que precisa ser chamada no
    codigo real. Ela usa a tela_atual para
    decidir qual funcao de tela deve ser
    chamada.
    Deve ser chamada na loop().
*/
void tela_inicio(void);
/*
	Tela de boas-vindas
*/
void tela_menu(byte tela);
/*
	Primeiro menu:
    [0] Ver sensor
    [1] Ver potenciometro
    [2] Historico (quantas vezes a sirene apitou)
    [3] Configuracoes
*/
void tela_ver_sensor(void);
/*
	Mostra a leitura de todos os sensores que tiverem
    sensor_chave = true (nos que tiverem false sera 
    mostrado "off" no lugar).
    Eh mostrado tambem a media_total
*/
void tela_ver_pot(void);
/*
	Mostra a leitura de todos os potenciometros,
    a tolerancia (eeprom) e o valor do potenciometro
    ideal (eeprom)
*/
void tela_historico(void);
/*
	Mostra na tela quantas vezes a sirene foi
    disparada (ep.contador).
*/
void tela_config(int tela);
/*
	Segundo menu:
    [0] Sensibilidade
    [1] Sensor on-off
    [2] Modo de variacao
    [3] Potenciometro ideal
    [4] Resetar configuracoes
*/
void tela_sensibilidade(void);
/*
	Permite alterar ep.tolerancia
*/
void mod_tolerancia(char c);
/*
	Funcao definida no codigo original
    Usada em tela_sensibilidade
*/
void tela_ligar_sensor(void);
/*
	Permite alterar ep.sensor_chave
*/
void mod_chave(int sensor, bool status);
/*
	Funcao definida no codigo original
    Usada em tela_ligar_sensor
*/
void tela_modo_variacao(void);
/*
	Ainda nao esta definido como os modos
    apresentados irao funcionar.
    Esta funcao apenas mostra texto na tela
*/
void tela_potenc_ideal(void);
/*
	Permite alterar ep.potenciometro_ideal
*/
void mod_pot_ideal(int pot, char botao);
/*
	Funcao definida no codigo original
    Usada em tela_potenc_ideal
*/
void tela_resetar_config(void);
/*
	Permite alterar todo o eeprom para as
    configuracoes originais
*/
void conf_padrao(void);
/*
	Funcao definida no codigo original
    Usada em tela_resetar_config
*/
/* ------------------------------------------ */

LiquidCrystal lcd(2, 3, 11, 10, 9, 8);

// Caractere definido para a seta
byte seta[8] = {
  B00000,
  B00100,
  B11110,
  B11111,
  B11110,
  B00100,
  B00000,
};

/* ------- Variaveis deste codigo ------- */
byte tela_atual = TELA_INICIO;
/*
	Variavel usada para determinar 
    qual funcao sera executada
*/
byte botao_porta[4] = {PORTA_UP, PORTA_DOWN, PORTA_ENTER, PORTA_BACK};
/*
	Define quais sao as portas do
    arduino onde estao os botoes
*/
int  posicao_escolha = 0;
/*
	Conforme o usuario vai navegando pelo
    menu, esta variavel eh alterada. Ela
    corresponde ao numero da escolha.
*/
bool limpar_linha = true;
/*
	Flag utilizada na funcao limpa(int linha).
    Ela garante que a linha escolhida sera
    substiuida por espacos em branco uma unica
    vez, garantindo que a tela nao fique piscando.
*/


/* ------ Textos mostrados na tela -------- */
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
/* ---------------------------------------- */


/* ---------- Utilitarios -----------------*/
void mostrar_seta(void)
{
  lcd.setCursor(0, posicao_escolha%2);
  lcd.write(byte(0)); // imprime o caractere da seta
  lcd.print(" ");
  lcd.setCursor(0, !(posicao_escolha%2));
  lcd.print("  ");
  
  /*
  	  Quando eh tirado o modulo 2 os valores possiveis
      sao 0 ou 1, exatamente o numero das linhas.
      A ! inverte o valor da linha onde vai ser impressa
      a seta, ou seja em uma linha eh impressa uma seta
      e na outra um espaco em branco
  */
}
void limpa(byte linha)
{
  if (limpar_linha)
  {
  	lcd.setCursor(0, linha);
  	lcd.print("                ");
    limpar_linha = false;
  }
  /*
  	  Se a flag limpar_tela for verdadeira a linha
      informada eh substituida por espacos em branco
      e a flag se torna falsa. Isso garante que a linha
      sera substuida apenas uma vez, evitando de o texto
      ficar piscando na tela.
  */
}
bool ler_botao(int indice)
{
  static bool estado1, estado2;
  /*
  	Neste codigo, estado precisa ser static mas 
    provavelmente no codigo real nao
  */
  static unsigned long tempo = millis();
  
  
  /* --------------- Debounce --------------- */
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
/* --------------------------------------- */
void LCD_menu_start(void)
{
  lcd.createChar(0, seta);
  lcd.noCursor();

  /* Esta funcao escolhe qual sera chamada */
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
    case TELA_HISTORICO:
    	tela_historico();
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
    case TELA_SENSOR_ON_OFF:
    	tela_ligar_sensor();
    	break;
    case TELA_MODO_VARIACAO:
    	tela_modo_variacao();
    	break;
    case TELA_POTENC_IDEAL:
    	tela_potenc_ideal();
    	break;
	case TELA_RESETAR_CONFIG:
    	tela_resetar_config();
    	break;    
  }
}
void tela_inicio(void)
{
  /* Tela de boas vindas */
  lcd.setCursor(0, 0);
  lcd.print(texto_inicio[0]);
  lcd.setCursor(0, 1);
  lcd.print(texto_inicio[1]);
 
  if (ler_botao(ENTER))
    tela_atual = TELA_MENU1;
  /* Apenas o botao ENTER eh reconhecido */
}
void tela_menu(byte tela)
{
  mostrar_seta();
  
  /* Duas possiveis telas */
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
  /* --------------------- */

  /* ------- Navegacao ------- */
  if (ler_botao(UP))
  {
    posicao_escolha--;
      
    if (posicao_escolha == 1)
      	tela_atual = TELA_MENU1;
    
    if (posicao_escolha == -1)
      posicao_escolha = 0;
  }
  else if (ler_botao(DOWN))
  {
    posicao_escolha++;
      
    if (posicao_escolha == 2)
      	tela_atual = TELA_MENU2;
    
    if (posicao_escolha == 4)
      posicao_escolha = 3;
  }
  /* -------------------------- */
  
  /* --------- Escolha -------- */
  if (ler_botao(ENTER))
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
  /* -------------------------- */
  
  /* -------- Retorno --------- */
  if (ler_botao(BACK))
  {
    posicao_escolha = 0;
    tela_atual = TELA_INICIO;
  }
  /* -------------------------- */
}
void tela_ver_sensor(void)
{
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  /* ------- Mostra valores ------- */
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
  /* ----------------------------- */
  
  limpa(1); // limpa a linha 1
  
  /* --- Mostra a media_total ---- */
  lcd.setCursor(0, 1);
  lcd.print("Media ");
  lcd.setCursor(6, 1);
  lcd.print(media_total);
  /* ----------------------------- */
  
  
  /* ------- Retorno ------ */
  if (ler_botao(BACK))
  {
    tela_atual = TELA_MENU1;
    limpar_linha = true;
  }
  /* ---------------------- */
}
void tela_ver_pot(void)
{
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  /* ------- Mostra valores ------- */
  for (int i = 0; i < 4; i++)
  {
    lcd.setCursor(4*i, 0);
    lcd.print(potenciometro_sinal[i]);
    lcd.print("  ");
  }
  /* ------------------------------ */
  
  /* Mostra a tolerancia e o valor ideal */
  lcd.setCursor(0, 1);
  lcd.print("Tol ");
  lcd.print(ep.tolerancia);
  lcd.print(" Ideal ");
  lcd.print(ep.potenciometro_ideal[0]);
  /* ---------------------------------- */
  
  /* ------- Retorno ------ */
  if (ler_botao(BACK))
  	tela_atual = TELA_MENU1;
  /* ---------------------- */
}
void tela_historico(void)
{
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  lcd.setCursor(0, 0);
  lcd.print(texto_historico[0]);
  
  limpa(1); // limpa a linha 1
  
  /* Mostra o valor do contador */
  lcd.setCursor(0, 1);
  lcd.print(ep.contador);
  /* -------------------------- */
  
  /* ------- Retorno ------ */
  if (ler_botao(BACK))
  {
    tela_atual = TELA_MENU1;
    limpar_linha = true;
  }
  /* ---------------------- */
}
void tela_config(int tela)
{
  mostrar_seta();
  
  // Tem 3 telas possiveis
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
  
  
  /* ----------- Navegacao -----------*/
  if (ler_botao(UP))
  {
    posicao_escolha--;
    
    if (posicao_escolha == 1)
      tela_atual = TELA_CONFIG1;
    if (posicao_escolha == 3)
      tela_atual = TELA_CONFIG2;
    
    if (posicao_escolha == -1)
		posicao_escolha = 0; 
  }
  else if (ler_botao(DOWN))
  {
    posicao_escolha++;
    
    if (posicao_escolha == 2)
      tela_atual = TELA_CONFIG2;
    if (posicao_escolha == 4)
      tela_atual = TELA_CONFIG3;
    
    if (posicao_escolha == 5)
      posicao_escolha = 4;
  }
  /* -------------------------------- */
  
  /* ----------- Escolha ------------ */
  else if (ler_botao(ENTER))
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
  /* -------------------------------- */
  
  /* ----------- Retorno ------------ */
  else if (ler_botao(BACK))
  {
    tela_atual = TELA_MENU1;
    posicao_escolha = 0;
  }
  /* -------------------------------- */
}

void tela_sensibilidade(void)
{
  bool executa = false;
  /*
  	A flag executa garante que o valor sera
    alterado uma vez a cada leitura do botao.
    Ela eh necessaria para evitar que, ao 
    pressionar o botao, o numero cresca ou 
    diminua indefinidamente.
  */
  char alteracao;
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  limpa(0); // limpa a linha 0
  
  /* ----- Mostra valor na tela ----- */
  lcd.setCursor(0, 0);
  lcd.print(texto_sensibilidade[0]);
  lcd.setCursor(6, 0);
  lcd.print(ep.tolerancia);
  lcd.setCursor(0, 1);
  lcd.print(texto_sensibilidade[1]);
  /* -------------------------------- */
  
  
  /* Le os botoes para definir a alteracao */
  if (ler_botao(UP))
  {
    alteracao = '+';
    executa = true;
  }
  else if (ler_botao(DOWN))
  {
    alteracao = '-';
    executa = true;
  }
  /* ------------------------------------- */
  
  if (executa)
  {
  	mod_tolerancia(alteracao);
    executa = false;
  }
  
  /* ------- Retorno ------ */
  if (ler_botao(BACK))
  {
    tela_atual = TELA_CONFIG1;
    limpar_linha = true;
  }
  /* ---------------------- */
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
void tela_ligar_sensor(void)
{
  /* 
  	Esta funcao mostra os estados de todos os
    sensores numa unica tela. A navegacao eh
    identica a dos outros menus com a diferenca
    que para alterar um valor, deve-se apertar ENTER
    (o caractere '>' sera substituido por '#') e usar
    os mesmos botoes de navegacao.
  */
  
  
  static char *icone = ">";
  static bool iniciar_alteracao = false;
  /*
  	Flag para realizar a funcao apenas uma vez
    a cada pressionar do botao
  */
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  /* --- Mostra texto e o cursor na tela --- */
  lcd.setCursor(0, 0);
  lcd.print(texto_ligar_sensor[0]);
  
  limpa(1); // limpa a linha 1
  
  lcd.setCursor(4 * posicao_escolha, 0);
  lcd.print(icone);
  /* --------------------------------------- */
  
  /* --- Mostra os estados dos sensores ---- */
  for (int i = 0; i < NUM_SENSOR;  i++)
  {
    lcd.setCursor(4 * i + 1, 1);
    if (ep.sensor_chave[i])
    	lcd.print(" on");
    else
      	lcd.print("off");
  }
  /* --------------------------------------- */
  
  /* ------------- Navegacao --------------- */
  if (!iniciar_alteracao)
  {
  	if (ler_botao(UP) && posicao_escolha < 3)
    	posicao_escolha++;
  	if (ler_botao(DOWN) && posicao_escolha > 0) 
    	posicao_escolha--;
    /*
    	Os conectivos logicos evitam quem a 
        variavel posicao_escolha receba valores
        indevidos.
    */
  }
  /* --------------------------------------- */
  
  /* ------- Escolha ------- */
  if (ler_botao(ENTER))
  {
    icone = "#";
    // o icone eh alterado para indicar
    // a escolha
    iniciar_alteracao = true;
  }
  /* ----------------------- */
  
  /* ------------ Alteracao ------------ */
  if (iniciar_alteracao)
  {
    if (ler_botao(UP))
		mod_chave(posicao_escolha, true);
    else if (ler_botao(DOWN))
		mod_chave(posicao_escolha, false);
      
    if (ler_botao(BACK))
    {
      icone = ">";
      iniciar_alteracao = false;
      posicao_escolha = 0;
    }
  }
  /* ----------------------------------- */
  
  /* ------- Retorno ------ */
  else if (ler_botao(BACK))
  {
   	tela_atual = TELA_CONFIG1;
   	limpar_linha = true;
  }
  /* ---------------------- */
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
  
  /* ------------ Navegacao -----------*/
  if (ler_botao(UP) && posicao_escolha == 1)
    posicao_escolha = !posicao_escolha;
  else if (ler_botao(DOWN) && posicao_escolha == 0)
    posicao_escolha = !posicao_escolha;  
  /* --------------------------------- */
  
  /* -------- Retorno -------- */
  if (ler_botao(BACK))
    tela_atual = TELA_CONFIG1;
  /* ------------------------- */
}

void tela_potenc_ideal(void)
{
  /*
  	Esta tela funciona exatamete como
    a tela_ligar_sensor
  */
  
  static char *icone = ">";
  static bool iniciar_alteracao = false;
  /*
  	Flag para realizar a funcao apenas uma vez
    a cada pressionar do botao
  */
  t_eeprom ep;
  EEPROM.get(0, ep);
  
  /* --- Mostra texto e o cursor na tela --- */
  lcd.setCursor(0, 0);
  lcd.print(texto_potenc_ideal[0]);
  
  limpa(1); // limpa a linha 1
  
  lcd.setCursor(4 * posicao_escolha, 0);
  lcd.print(icone);
  /* --------------------------------------- */
  
  /* Mostra as leituras dos potenciometros */
  for (int i = 0; i < NUM_SENSOR;  i++)
  {
    lcd.setCursor(4 * i + 1, 1);
    lcd.print(ep.potenciometro_ideal[i]);
  }
  /* --------------------------------------- */
  
  /* ------------- Navegacao --------------- */
  if (!iniciar_alteracao)
  {
  	if (ler_botao(UP) && posicao_escolha < 3)
    	posicao_escolha++;
  	if (ler_botao(DOWN) && posicao_escolha > 0) 
    	posicao_escolha--;
    /*
    	Os conectivos logicos evitam quem a 
        variavel posicao_escolha receba valores
        indevidos.
    */
  }
  /* --------------------------------------- */
  
  /* ------- Escolha ------- */
  if (ler_botao(ENTER))
  {
    seta = "#";
    iniciar_alteracao = true;
  }
  /* ----------------------- */
  
  /* ------------ Alteracao ------------ */
  if (iniciar_alteracao)
  {
    if (ler_botao(UP))
      	mod_pot_ideal(posicao_escolha, '+');
    else if (ler_botao(DOWN))
      	mod_pot_ideal(posicao_escolha, '-');
      
    else if (ler_botao(BACK))
    {
      seta = ">";
      iniciar_alteracao = false;
      posicao_escolha = 0;
    }
  }
  /* ----------------------------------- */
  
  /* ------- Retorno ------ */
  else if (ler_botao(BACK))
  {
   	tela_atual = TELA_CONFIG1;
   	limpar_linha = true;
  }
  /* ---------------------- */
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
  /* -------Mostra texto --------- */
  lcd.setCursor(0, 0);
  lcd.print(texto_resetar_config[0]);
  lcd.setCursor(0, 1);
  lcd.print(texto_resetar_config[1]);
  /* ----------------------------- */
  
  /* --------- Escolha ----------- */
  if (ler_botao(ENTER))
  {
    conf_padrao();
    tela_atual = TELA_INICIO;
  }
  /* ----------------------------- */
  
  /* ------ Retorno -------*/
  else if (ler_botao(BACK))
    tela_atual = TELA_CONFIG1;
  /* --------------------- */
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
