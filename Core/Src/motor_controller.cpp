#include "motor_controller.hpp"

MotorController::MotorController(TIM_HandleTypeDef* htim, uint32_t channel, MotorMode mode): PwmController(htim, channel) {

    // モードを設定
    m_mode = mode;

    // デフォルトのパルス幅範囲を設定（1000~2000us）
    m_min_pulse = 1000;
    m_max_pulse = 2000;

    // ニュートラル位置を計算
    m_neutral_pulse = (m_min_pulse + m_max_pulse) / 2;
}

MotorController::~MotorController() {

}

uint8_t MotorController::isInitialized() {

    return PwmController::isInitialized();
}

void MotorController::setPulseRange(uint32_t min_pulse, uint32_t max_pulse) {

    m_min_pulse = min_pulse;
    m_max_pulse = max_pulse;

    // ニュートラル位置を再計算
    m_neutral_pulse = (m_min_pulse + m_max_pulse) / 2;
}

uint8_t MotorController::setSpeed(float speed_percent) {

    // 速度の範囲チェック（モードによって異なる）
    if (m_mode == MotorMode::REVERSE) {

        // リバースモード: -100 ~ 100 %
        if (speed_percent < REVERSE_MIN_SPEED || speed_percent > REVERSE_MAX_SPEED) {

            return 1;
        }
    }
    else {

        // 通常モード: 0 ~ 100 %
        if (speed_percent < MIN_SPEED || speed_percent > MAX_SPEED) {

            return 1;
        }
    }

    // 速度をパルス幅に変換
    uint32_t pulse_width_us = speedToParseWidth(speed_percent);

    // パルス幅を設定
    return setPulseWidth(pulse_width_us);
}

uint8_t MotorController::stop() {

    return PwmController::stop();
}

uint32_t MotorController::speedToParseWidth(float speed_percent) {

    uint32_t pulse_width_us;

    if (m_mode == MotorMode::REVERSE) {

        // リバースモード
        // -100% = 1000us（完全速進）
        // 0% = 1000us（停止）
        // 100% = 2000us（完全前進）
        if (speed_percent <= 0.0f) {

            // 速度が0以下の場合は中立位置
            pulse_width_us = m_neutral_pulse;
        }
        else {

            // 速度が0以上の場合、中立位置から最大パルス幅までをスケール
            uint32_t pulse_range = m_max_pulse - m_neutral_pulse;
            pulse_width_us = (uint32_t)(m_neutral_pulse + (speed_percent / 100.0f) * pulse_range);
        }
    }
    else {

        // 通常モード
        // 0% = 1000us、100% = 2000us
        uint32_t pulse_range = m_max_pulse - m_min_pulse;
        pulse_width_us = (uint32_t)(m_min_pulse + (speed_percent / 100.0f) * pulse_range);
    }

    return pulse_width_us;
}
