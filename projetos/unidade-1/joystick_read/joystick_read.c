#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

// Configurações
#define JOYSTICK_Y_ADC_PIN 26  // Eixo Y
#define JOYSTICK_X_ADC_PIN 27  // Eixo X
#define I2C_SDA 14
#define I2C_SCL 15
#define DEADZONE_CENTER 2048
#define DEADZONE_RANGE 200 // Deadzone para evitar variação no joystick parado

// Inicializa ADC
void init_adc() {
    adc_init();
    adc_gpio_init(JOYSTICK_X_ADC_PIN);
    adc_gpio_init(JOYSTICK_Y_ADC_PIN);
}

// Lê canal ADC
uint16_t read_adc(uint channel) {
    adc_select_input(channel);
    return adc_read();
}

// Aplica deadzone
uint16_t apply_deadzone(uint16_t raw_val) {
    int16_t offset = raw_val - DEADZONE_CENTER;
    if (abs(offset) < DEADZONE_RANGE) {
        return DEADZONE_CENTER; // Mantém o valor central
    }
    return raw_val;
}

// Atualiza display OLED
void atualizar_display(uint8_t *buffer, struct render_area *area, uint16_t x_val, uint16_t y_val) {
    char linha1[32], linha2[32];

    memset(buffer, 0, ssd1306_buffer_length);

    snprintf(linha1, sizeof(linha1), "Eixo X: %4d", x_val);
    snprintf(linha2, sizeof(linha2), "Eixo Y: %4d", y_val);

    ssd1306_draw_string(buffer, 5, 0, "Joystick BitDogLab");
    ssd1306_draw_string(buffer, 5, 16, linha1);
    ssd1306_draw_string(buffer, 5, 32, linha2);

    render_on_display(buffer, area);
}

// Função principal
int main() {
    stdio_init_all();
    init_adc();

    // Inicializa I2C
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();

    struct render_area area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&area);

    uint8_t buffer[ssd1306_buffer_length];
    memset(buffer, 0, sizeof(buffer));

    ssd1306_draw_string(buffer, 5, 0, "Inicializando...");
    render_on_display(buffer, &area);
    sleep_ms(500);

    // Loop principal
    while (true) {
        uint16_t raw_x = read_adc(1); // X 
        uint16_t raw_y = read_adc(0); // Y

        uint16_t filtered_x = apply_deadzone(raw_x);
        uint16_t filtered_y = apply_deadzone(raw_y);

        atualizar_display(buffer, &area, filtered_x, filtered_y);
        sleep_ms(50);
    }

    return 0;
}
