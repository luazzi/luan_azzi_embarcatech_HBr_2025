#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

// Configurações do I2C para OLED
#define I2C_SDA 14
#define I2C_SCL 15
#define ADC_TEMPERATURE_CHANNEL 4  // Canal do sensor de temperatura interno

// Função para converter ADC para temperatura
float adc_to_temperature(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = adc_value * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

// Função para atualizar o display OLED
void atualizar_display(uint8_t *buffer, struct render_area *area, float temperatura) {
    char linha1[32], linha2[32];

    memset(buffer, 0, ssd1306_buffer_length);

    snprintf(linha1, sizeof(linha1), "Temp. Interna:");
    snprintf(linha2, sizeof(linha2), "%.2f C", temperatura);


    ssd1306_draw_string(buffer, 5, 16, linha1);
    ssd1306_draw_string(buffer, 5, 32, linha2);

    render_on_display(buffer, area);
}

int main() {
    stdio_init_all();

    // Inicializa ADC para sensor de temperatura
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(ADC_TEMPERATURE_CHANNEL);

    // Inicializa I2C
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa display OLED
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

    // Exibe mensagem inicial
    ssd1306_draw_string(buffer, 5, 0, "Inicializando...");
    render_on_display(buffer, &area);
    sleep_ms(500);

    absolute_time_t ultima_atualizacao = get_absolute_time();

    while (true) {
        // Lê temperatura a cada segundo
        if (absolute_time_diff_us(ultima_atualizacao, get_absolute_time()) > 1000000) {
            uint16_t adc_value = adc_read();
            float temperatura = adc_to_temperature(adc_value);
            
            // Atualiza display
            atualizar_display(buffer, &area, temperatura);
            
            // Também imprime no serial (opcional)
            printf("Temperatura: %.2f C\n", temperatura);
            
            ultima_atualizacao = get_absolute_time();
        }

        sleep_ms(10);  // Pequeno delay para evitar sobrecarga
    }

    return 0;
}