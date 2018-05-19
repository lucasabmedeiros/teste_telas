/*
  Este arquivo é apenas um teste para a
  criacao do menu interativo do SHIU
*/

#include <LiquidCrystal.h>

typedef struct t_eeprom
{
    int contador;
    int tolerancia;
    bool sensor_chave[NUM_SENSOR];        
    short potenciometro_ideal[NUM_SENSOR];
    byte tela_atual;    // adicionado para utilizar as telas
}t_eeprom;

LiquidCrystal lcd(2, 3, 11, 10, 9, 8); // numeros arbitrarios

void setup()
{
}

void loop()
{
}
