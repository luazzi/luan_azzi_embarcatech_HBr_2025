# Leitura dos Sinais Analógicos do Joystick
Nesta tarefa, foi feito um código para ler os dados de posição do joystick da BitDogLab e mostrá-los no dispay OLED.

## Instruções da tarefa
Faça um programa em C para ler os valores convertidos digitalmente do joystick da BitDogLab. Os valores podem ser mostrados no terminal ou então no display OLED. 

## Lista de Materiais
| Componente            | GPIO                                |
|-----------------------|------------------------------------ |
| Joystick              | Eixo Y: GPIO26 / Eixo X: GPIO27     |
| Display OLED I2C      | SDA: GPIO14 / SCL: GPIO15           |

## Arquivos
- `joystick_read.c`: Arquivo com o código principal;

- `inc`: Arquivo com a biblioteca utilizada para configuração do display OLED.

- `CMakeLists.txt`: Arquivo de configuração para compilação com o SDK do Pico.

## Comentários

### Problema de precisão de centro
Durante os testes iniciais, observou-se que o joystick apresentava imprecisão no retorno à posição central após movimentação. As leituras ADC no repouso variaviam em torno do valor teórico central (2048), com flutuações típicas de ±150 unidades, mesmo com o joystick fisicamente centralizado.
  
### Solução implementada:
Foi introduzido um algoritmo de deadzone que:
1. Considera qualquer valor entre (CENTRO ± DEADZONE_RANGE) como posição central
2. Mapeia estes valores para exatamente 2048 (valor central ideal)
3. Mantém a resposta linear fora da zona morta

### Parâmetros

1. DEADZONE_RANGE: 200
2. DEADZONE_CENTER: 2048


