#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

// Configurações
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define I2C_SDA      14
#define I2C_SCL      15

// Variáveis globais
volatile int contador = 0;
volatile int cliques_botao_b = 0;
volatile bool contagem_ativa = false;
volatile bool atualizar_display_necessario = false;
volatile absolute_time_t proximo_tick;


// Inicialização dos botões
void init_button(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

// Callback de interrupção dos botões
void button_callback(uint gpio, uint32_t events) {

    if (gpio == BUTTON_A_PIN) {
        contador = 9;
        cliques_botao_b = 0;
        contagem_ativa = true;
        atualizar_display_necessario = true;
        proximo_tick = make_timeout_time_ms(1000);  // Reinicia o temporizador
    } else if (gpio == BUTTON_B_PIN && contagem_ativa && contador > 0) {
        cliques_botao_b++;
        atualizar_display_necessario = true;
    }
}

// Atualiza o display OLED 
void atualizar_display(uint8_t *buffer, struct render_area *area) {
    char linha1[32], linha2[32];

    memset(buffer, 0, ssd1306_buffer_length);

    snprintf(linha1, sizeof(linha1), "Tempo: %d", contador);
    snprintf(linha2, sizeof(linha2), "Cliques: %d", cliques_botao_b);

    ssd1306_draw_string(buffer, 5, 0, linha1);
    ssd1306_draw_string(buffer, 5, 16, linha2);

    render_on_display(buffer, area);
}

// Função principal
int main() {
    stdio_init_all();

    // Inicializa I2C
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa botões
    init_button(BUTTON_A_PIN);
    init_button(BUTTON_B_PIN);

    // Configura interrupções
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    // Inicializa display OLED
    ssd1306_init();

    struct render_area area = {
        .start_column = 0,
        .end_column   = ssd1306_width - 1,
        .start_page   = 0,
        .end_page     = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&area);

    uint8_t buffer[ssd1306_buffer_length];
    memset(buffer, 0, sizeof(buffer));

    // Exibe display inicial
    atualizar_display(buffer, &area);
    proximo_tick = make_timeout_time_ms(1000);

    // Loop principal
    while (true) {
        // Atualiza a cada segundo, se contagem ativa
        if (contagem_ativa && absolute_time_diff_us(get_absolute_time(), proximo_tick) <= 0) {
            if (contador > 0) {
                contador--;
                atualizar_display_necessario = true;
            } else {
                contagem_ativa = false;
                atualizar_display_necessario = true;
            }
            proximo_tick = make_timeout_time_ms(1000);
        }

        // Atualiza display sempre que necessário
        if (atualizar_display_necessario) {
            atualizar_display(buffer, &area);
            atualizar_display_necessario = false;
        }

        sleep_ms(10);
    }

    return 0;
}