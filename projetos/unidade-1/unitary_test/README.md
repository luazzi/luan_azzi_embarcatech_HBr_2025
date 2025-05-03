# Teste Unitário da Função do Monitor de Temperatura Interna do MCU
Nesta tarefa, foi feito um código para ler a temperatura interna do RP2040 e verificar se a função de conversão para °C retorna o valor correto.

## Instruções da tarefa

- Implemente a função float adc_to_celsius(uint16_t adc_val);

- Escreva uma função de teste unitário que verifica se a função retorna o valor correto (com margem de erro) para uma leitura simulada de ADC. Sugere-se o uso da biblioteca Unity para o teste unitário.

- Use um teste com valor de ADC conhecido (ex.: para 0.706 V, a temperatura deve ser 27 °C).
 
## Arquivos
- `app`: Arquivo com o código de leitura de temperatura;

- `unity`: Arquivo com a biblioteca unity;

- `test`: Arquivo com o código de teste;

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

### Teste unitário
Para realizar o teste unitário, foi inserido simulado um valor de 876 de ADC, que deve resultar 27°C. O resultado foi satisfatório e dentro da margem, portanto passou no teste.
