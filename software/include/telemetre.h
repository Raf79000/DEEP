#ifndef TELEMETRE_H
#define TELEMETRE_H

uint16_t HCSRO4_mesures(GPIO_TypeDef * TRIG_GPIO, uint16_t TRIG_PIN, GPIO_TypeDef * ECHO_GPIO, uint16_t ECHO_PIN);

#endif // TELEMETRE_H
