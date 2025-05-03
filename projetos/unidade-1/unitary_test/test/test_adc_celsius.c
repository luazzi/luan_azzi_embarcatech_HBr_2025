#include "include/unity.h"
#include "include/adc_to_celsius.h"
#include "pico/stdlib.h"

void setUp(void) {}

void tearDown(void){}

void test_adc_to_celsius_27C(void) 
{
    const uint16_t adc_value = 876;
    const float temperature = adc_to_celsius(adc_value);
    
    printf("Testando conversão ADC para 27°C...\n");
    printf("Valor ADC: %u, Temperatura calculada: %.2f°C\n", 
           adc_value, temperature);
    
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 27.0f,  temperature);
}

int main(void) 
{
    // Inicialização do hardware
    stdio_init_all();
    
    // Delay para estabilização 
    sleep_ms(3000);

    UNITY_BEGIN();
    
    printf("\n=== Teste Unitário - Conversor ADC para Temperatura ===\n");
    sleep_ms(2000);
    
    RUN_TEST(test_adc_to_celsius_27C);
    
    printf("\nTestes concluídos!\n");
    return UNITY_END();
}