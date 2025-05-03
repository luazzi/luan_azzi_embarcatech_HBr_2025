# Teste Unitário da Função do Monitor de Temperatura Interna do MCU
Nesta tarefa, foi reestruturado um código para utilizar a arquitetura modular.

## Instruções da tarefa

- Crie um driver led_embutido.c que use diretamente a API cyw43_arch.

- Implemente um hal_led.c com a função hal_led_toggle() que abstraia o driver.

- Escreva um main.c simples no diretório app/ que apenas use a função da HAL para piscar o LED.

 
## Arquivos
- `app`: Arquivo com a lógica da aplicação principal;

- `drivers`: Arquivo com os códigos de controle direto de hardware;

- `hal`: Camada de abstração que expõe funções simples e reutilizáveis;

- `include`: Arquivo com os cabeçalhos necessários;

- `CMakeLists.txt`: Arquivo de configuração para compilação com o SDK do Pico.

## Comentários

### Código utilizado de base

```c

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        return -1;
    }

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }
}

```



