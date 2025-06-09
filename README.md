# Sistema de Detecção de Incêndio IoT

Este projeto implementa um sistema de detecção de incêndio utilizando IoT, com sensores conectados a um ESP32 e integração com a plataforma ThingSpeak para monitoramento em tempo real. O protótipo pode ser testado tanto em hardware real quanto no simulador Wokwi.

---

## 🎯 Objetivos

- Monitorar ambientes quanto a temperatura, presença de gás e chama.
- Emitir alertas imediatos (luminosos e sonoros) em situações de risco.
- Transmitir dados em tempo real para a nuvem para visualização remota.
- Testar e validar a solução em ambiente físico ou simulado (Wokwi).
- Garantir a reprodutibilidade e documentar todas as etapas do projeto.


## Componentes Utilizados

### Hardware (Simulado ou Real)
- ESP32 DevKit
- Sensor de Temperatura DHT22
- Sensor de Gás MQ-2 (detecção de fumaça)
- LEDs (vermelho para alerta, verde para status normal)
- Buzzer para alarme sonoro

### Software e Serviços
- Arduino IDE
- Conta no ThingSpeak
- Simulador Wokwi (opcional)

---

## Estrutura do Projeto
```
├── src/
│   ├── FireAlertSystem.ino      # Código principal do ESP32
│   ├── diagram.json             # Diagrama do circuito para Wokwi
└── README.md
```

---

## Passo a Passo para Simulação

1. **Clone este repositório**
2. **Abra o [Wokwi](https://wokwi.com/)**
3. **Importe o arquivo `diagram.json`** para montar o circuito automaticamente
4. **Cole o código do `FireAlertSystem.ino`** na aba de código do Wokwi
5. **Inicie a simulação**
6. **Acesse o canal do ThingSpeak** para visualizar os dados em tempo real

---

## Funcionalidades
- Monitoramento contínuo de temperatura, gás e chama
- Envio de dados para ThingSpeak a cada 15 segundos
- Alerta visual (LED) e sonoro (buzzer) em caso de detecção de incêndio
- Dashboard no ThingSpeak para visualização em tempo real
- Histórico de leituras dos sensores

### Limites de Detecção
- Temperatura > 50°C
- Nível de gás > 400 ppm
- Detecção de chama: < 100 (valor analógico)

Se qualquer um desses limites for ultrapassado, o sistema ativa o alerta.

---

## Exemplo de Uso
1. Simule aumento de temperatura, gás ou chama no Wokwi
2. Veja o LED vermelho e o buzzer acionarem
3. Confira o status "Alerta" no dashboard do ThingSpeak

---

## Créditos
- Antonio Caue, Marcelo Siqueira e Felipe Orikasa
---

## Observações
- O projeto pode ser testado 100% online via Wokwi e ThingSpeak
