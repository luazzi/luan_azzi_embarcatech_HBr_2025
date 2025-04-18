# Monitor da Temperatura Interna da MCU
Nesta tarefa, foi feito um código para ler a temperatura interna do RP2040.

## Instruções da tarefa
Faça um programa em C para ler a temperatura interna do RP2040. Converta a leitura do ADC em um valor em ºC. 

## Lista de Materiais
| Componente                    | GPIO                                |
|-------------------------------|------------------------------------ |
| Leitor de temperatura interna |         ------------------          |
| Display OLED I2C              | SDA: GPIO14 / SCL: GPIO15           |

## Arquivos
- `temperature_reader.c`: Arquivo com o código principal;

- `inc`: Arquivo com a biblioteca utilizada para configuração do display OLED.

- `CMakeLists.txt`: Arquivo de configuração para compilação com o SDK do Pico.

## Comentários

### Função para converter o sinal do ADC em temperatura
Para converter o ADC em °C, utilizou-se a função disponível no datasheet do RP2040:
```c

float adc_to_temperature(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = adc_value * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

```

### Variação de Temperatura
Há variações grandes de temperatura em um curto período. Seria interessante fazer um estudo sobre a precisão dessa temperatura.

