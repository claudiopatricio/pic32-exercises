Algumas coisas a ter em consideração no teste prático

LEDS: RE0 a RE7 (0x00FF)
Displays: RB8 a RB14 (0x7F00)
Display_high: RD5 (display direito)
Display_low: RD6 (display esquerdo)
Switches: RB0 a RB3 (0x000F)

TRISX: 0->Output | 1->Input
PORTX: Ler
LATX: Escrever

Exemplo1.c: Ler do switch e escrever o mesmo valor nos leds 0 a 4
Exemplo2.c: Ler do switch e escrever o mesmo valor em hexadecimal no display do lado direito

Exemplo1-pic.c: Exercicio1 a funcionar na placa das praticas
Exemplo2-pic.c: Exercicio2 a funcionar na placa das praticas
