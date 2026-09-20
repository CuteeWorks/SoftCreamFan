// SoftCreamFan.ino
// UIAPduino Pro Micro CH32V003
const uint8_t MOTOR_PIN = D9;
const uint8_t BUTTON_PIN = D10;
const uint8_t BUZZER_PIN = D5;

const uint16_t MOTOR_PWM_ON_MILLIS = 1;
const uint16_t MOTOR_PWM_OFF_MILLIS = 15;
const uint16_t MOTOR_PWM_PERIOD = MOTOR_PWM_ON_MILLIS + MOTOR_PWM_OFF_MILLIS;

// チャタリング対策用のデバウンス時間
const uint16_t DEBOUNCE_MS = 40;

// ---- ブザー(メロディ)関連 ----
struct Note {
  uint16_t freq; // Hz (0 = 休符)
  uint16_t duration_ms; // この音を鳴らす長さ
};

const Note MELODY[] = {
  {262, 250},
  {0, 125},
  {262, 125},
  {330, 250},
  {0, 125},
  {330, 125},
  {440, 250},
  {0, 125},
  {392, 375},
  {0, 125},
  {440, 375},
  {0, 125},
  {494, 125},
  {523, 250},
  {0, 125},
  {440, 125},
  {392, 375},
  {0, 625},
  {349, 250},
  {0, 125},
  {349, 375},
  {0, 125},
  {349, 125},
  {330, 250},
  {0, 125},
  {294, 125},
  {262, 250},
  {0, 250},
  {294, 62},
  {0, 312},
  {294, 375},
  {0, 125},
  {330, 125},
  {294, 500},
  {0, 500},
  {262, 250},
  {0, 125},
  {262, 125},
  {330, 250},
  {0, 125},
  {330, 125},
  {440, 250},
  {0, 125},
  {392, 375},
  {0, 125},
  {440, 375},
  {0, 125},
  {494, 125},
  {523, 250},
  {0, 125},
  {440, 125},
  {392, 375},
  {0, 125},
  {330, 125},
  {294, 125},
  {262, 125},
  {0, 500},
  {262, 375},
  {0, 125},
  {262, 125},
  {294, 250},
  {0, 125},
  {294, 125},
  {262, 250},
  {0, 125},
  {247, 125},
  {262, 1000},
  {0, 812},
  {262, 125},
  {0, 62},
  {262, 250},
  {0, 125},
  {294, 125},
  {330, 250},
  {0, 125},
  {330, 125},
  {392, 250},
  {0, 125},
  {392, 125},
  {247, 250},
  {0, 125},
  {247, 375},
  {0, 125},
  {247, 125},
  {220, 250},
  {0, 125},
  {220, 125},
  {349, 250},
  {0, 125},
  {349, 125},
  {330, 250},
  {0, 250},
  {262, 250},
  {0, 125},
  {294, 125},
  {330, 250},
  {0, 250},
  {392, 250},
  {0, 125},
  {523, 125},
  {494, 312},
  {0, 188},
  {392, 250},
  {0, 125},
  {330, 125},
  {440, 812},
  {392, 125},
  {0, 62},
  {392, 312},
  {0, 688},
  {220, 312},
  {0, 62},
  {262, 125},
  {349, 312},
  {0, 62},
  {440, 125},
  {392, 250},
  {0, 250},
  {494, 250},
  {0, 125},
  {523, 625},

  {0, 0} /* END */
};

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

int32_t updateAudio(int32_t prev_state, uint32_t delta) {
  uint32_t accumulated_ms = 0;
  int32_t i = 0;

  if (prev_state == -1) {
    if (delta > MELODY[0].duration_ms) {
      return -1;
    }

    tone(BUZZER_PIN, MELODY[0].freq);
    return 0;
  }

  while (true) {
    const Note *ptr = MELODY + i;

    if (ptr->freq == 0 && ptr->duration_ms == 0) {
      /* END */
      noTone(BUZZER_PIN);
      return -1;
    }

    accumulated_ms += ptr->duration_ms;
    if (accumulated_ms > delta) {
      if (i != prev_state) {
        /* MELODY index change */
        if (ptr->freq) {
          /* 再生開始時、またトーン変更時に帯域外スペクトラムが発生していると
           * 考えられるが聞き取りにはさほど影響が無いよう。 - memetan */
          tone(BUZZER_PIN, ptr->freq);
        } else {
          noTone(BUZZER_PIN);
        }
      }

      return i;
    }

    i++;
  }
}

uint32_t last_btn_event_time = 0;
int64_t btn_time = -1;
int32_t audio_state = -1;

void loop() {
  bool btn_state = (digitalRead(BUTTON_PIN) == LOW);

  if (last_btn_event_time + DEBOUNCE_MS < millis()) {
    if (btn_state && btn_time == -1) {
      btn_time = millis();
      last_btn_event_time = millis();
    } else if (!btn_state && btn_time != -1) {
      noTone(BUZZER_PIN);
      digitalWrite(MOTOR_PIN, LOW);

      btn_time = -1;
      last_btn_event_time = millis();
    }
  }
  
  if (btn_time == -1) return;

  uint32_t delta = millis() - btn_time;
  uint32_t motor_pwm_phase = delta % MOTOR_PWM_PERIOD;

  digitalWrite(MOTOR_PIN, motor_pwm_phase < MOTOR_PWM_ON_MILLIS ? HIGH : LOW);
  audio_state = updateAudio(audio_state, delta);
}
