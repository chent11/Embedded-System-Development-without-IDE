#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
  OFF = 0,
  ON
} ledState;

void ledInit(void);
void ledRed(ledState isOn);

#ifdef __cplusplus
}
#endif

#endif
