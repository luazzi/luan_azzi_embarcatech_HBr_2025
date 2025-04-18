# Contador Decrescente com Registro de Eventos por Interrupção
Nesta tarefa, utiliza-se a BitDogLab para implementar um contador decrescente controlado por interrupção.

## Instruções da tarefa
Faça um programa, em linguagem C, que implemente um contador decrescente controlado por interrupção, com o seguinte comportamento:

Toda vez que o Botão A (GPIO5) for pressionado:

1. O contador decrescente reinicia em 9 e o valor da contagem é mostrado no display OLED.
2. O sistema entra em modo de contagem regressiva ativa, decrementando o contador de 1 em 1 a cada segundo até chegar em zero.
3. Durante essa contagem (ou seja, de 9 até 0), o programa deve registrar quantas vezes o Botão B (GPIO6) foi pressionado. O valor deste registro de eventos de botão pressionado também deve ser mostrado no display OLED.
4. Quando o contador atingir zero, o sistema congela e ignora temporariamente os cliques no Botão B (eles não devem ser acumulados fora do intervalo ativo).

O sistema permanece parado após a contagem, exibindo:

1. O valor 0 no contador
2. A quantidade final de cliques no Botão B registrados durante o período de 9 segundo (contagem regressiva)

Somente ao pressionar novamente o Botão A, o processo todo se reinicia:

1. O contador volta para 9
2. O número de cliques do Botão B é zerado
3. A contagem recomeça do início

## Lista de Materiais
| Componente            | GPIO                                |
|-----------------------|------------------------------------ |
| Botão A               | GPIO5                               |
| Botão B               | GPIO6                               |
| Display OLED I2C      | SDA: GPIO14 / SCL: GPIO15           |

## Arquivos
- `counter.c`: Arquivo com o código principal;

- `inc`: Arquivo com a biblioteca utilizada para configuração do display OLED.

- `CMakeLists.txt`: Arquivo de configuração para compilação com o SDK do Pico.

## Comentários

### Bounce dos Botões
Durante os testes, observou-se que os botões físicos apresentam bounce (múltiplas transições rápidas) quando pressionados, causando registros múltiplos em um mesmo clique. Para resolver esse problema, é possível usar um mecanismo de debounce, como uma lógica de temporização para ignorar transições muito rápidas.

Observações: essa mudança não foi implementada no código, mas pode ser implementada adicionando duas variáveis:

```c
volatile absolute_time_t ultimo_botao_a = 0;
volatile absolute_time_t ultimo_botao_b = 0;
```

E substituindo a função de callback por:

```c
void button_callback(uint gpio, uint32_t events) {
    absolute_time_t now = get_absolute_time();
    
    if (gpio == BUTTON_A_PIN) {
        if (absolute_time_diff_us(ultimo_botao_a, now) > DEBOUNCE_DELAY_MS * 1000) {
            contador = 9;
            cliques_botao_b = 0;
            contagem_ativa = true;
            atualizar_display_necessario = true;
            proximo_tick = make_timeout_time_ms(1000);
            ultimo_botao_a = now;
        }
    } else if (gpio == BUTTON_B_PIN) {
        if (absolute_time_diff_us(ultimo_botao_b, now) > DEBOUNCE_DELAY_MS * 1000) {
            if (contagem_ativa && contador > 0) {
                cliques_botao_b++;
                atualizar_display_necessario = true;
            }
            ultimo_botao_b = now;
        }
    }
}

```