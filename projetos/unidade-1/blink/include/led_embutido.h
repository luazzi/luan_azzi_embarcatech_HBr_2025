#ifndef LED_EMBUTIDO_H
#define LED_EMBUTIDO_H

// Inicializa o driver do LED embutido
int led_embutido_init(void);

// Define o estado do LED: 1 = ligado, 0 = desligado
void led_embutido_set(int);

#endif // LED_EMBUTIDO_H
