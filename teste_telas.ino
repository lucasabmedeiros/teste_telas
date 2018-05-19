/*
  Este arquivo é apenas um teste para a
  criacao do menu interativo do SHIU
*/

#include <LiquidCrystal.h>

#define TELA_INICIO 0
#define TELA_MENU_INICIAL1 1
#define TELA_MENU_INICIAL2 2

#define UP 1        //  
#define DOWN 2      //NUMEROS
#define ENTER 3     //ARBITRARIOS
#define BACK 4      //

void tela_escolha(t_eeprom ep);
bool ler_botao(void);
void tela_inicio(void);
void tela_menu_inicial(int escolha);

byte botao_porta[4] = {UP, DOWN, ENTER, BACK};
bool botao_estado[4];   // informa se um botao esta ou nao ligado
byte posicao_seta = 0;

typedef struct st_eeprom
{
    int contador;
    int tolerancia;
    bool sensor_chave[NUM_SENSOR];        
    short potenciometro_ideal[NUM_SENSOR];
    byte tela_atual;    // adicionado para utilizar as telas
}t_eeprom;

LiquidCrystal lcd = {5, 6, 7, 8, 9, 10} // numeros arbitrarios


void setup()
{
  t_eeprom ep;

  lcd.begin();
  lcd.nocursor();   // nao eh necessario mostrar o cursor na tela

  pinMode(botao_porta[UP], INPUT);
  pinMode(botao_porta[DOWN], INPUT);
  pinMode(botao_porta[ENTER], INPUT);
  pinMode(botao_porta[BACK], INPUT);

  ep.tela_atual = TELA_INICIO;
}

void loop()
{
  t_eeprom ep
  tela_escolha(ep);
}
void tela_escolha(t_eeprom ep)
{
  // Com certeza, isso vai ser mudado
  switch(ep.tela_atual)
  {
    case TELA_INICIO:
      tela_inicio();
      break;
    case TELA_MENU_INICIAL1:
    case TELA_MENU_INICIAL2:
      tela_menu_inicial(ep.tela_atual);
      break;
  }
}
bool ler_botao(byte botao)
{
    byte estado1, estado2, tempo, atraso = 10;

    estado1 = digitalRead(botao);

    tempo = millis();
    if (millis() - tempo > 10)
        estado2 = digitalRead(botao);

    if (estado1 == estado2)
        return true;
    return false;

/*
    Se a leitura do botao se mantiver apos
    10 milissegundos, significa que o botao
    foi realmente pressionado
*/
}
void tela_inicio(void)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bemvindo ao SHIU");
    lcd.setCursor(3, 1);
    lcd.print("Aperte ENTER");

    if (digitalRead(botao_porta[ENTER])
      tela_atual = TELA_MENU_INICIAL1;
}
void tela_menu_inicial(void)
{
  t_eeprom ep;

  lcd.clear();
  lcd.setCursor(0, posicao_seta);
  lcd.print(">");

  if (escolha == 1)
  {
    lcd.setCursor(0, 2);
    lcd.print("Ver sensores");
    lcd.setCursor(1, 2);
    lcd.print("Ver potenciome");
  }
  else if (escolha == 2)
  {
    lcd.setCursor(0, 2);
    lcd.print("Historico");
    lcd.setCursor(1, 2);
    lcd.print("Configuracoes");
  }

  if (ler_botao(botao_porta[UP]))
  {
    if (posicao_seta == 0 && ep.tela_atual == TELA_MENU_INICIAL1)
      ep.tela_atual = TELA_MENU_INICIAL2;
    else if (posicao_seta == 0 && ep.tela_atual == TELA_MENU_INICIAL2)
      ep.tela_atual = TELA_MENU_INICIAL1;

    posicao_seta = (posicao_seta + 1) % 2;
    // o modulo garante que vai ser sempre 0 ou 1 
  }
  else if (ler_botao(botao_porta[DOWN]))
  {
    if (posicao_seta == 1 %% ep.tela_atual == TELA_MENU_INICIAL1)
      ep.tela_atual = TELA_MENU_INICIAL2;
    else (posicao_seta == 1 && ep.tela_atual == TELA_MENU_INICIAL2)
      ep.tela_atual = TELA_MENU_INICIAL1;

    posicao_seta = (posicao_seta + 1) % 2;
    // o modulo garante que vai ser sempre 0 ou 1 
  }
}
