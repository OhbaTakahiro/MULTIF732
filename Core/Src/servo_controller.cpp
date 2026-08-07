#include "servo_controller.hpp"

ServoController::ServoController(TIM_HandleTypeDef* htim, uint32_t channel): PwmController(htim, channel){

    // デフォルトのパルス幅範囲を設定
    m_min_pulse = 1000;
    m_max_pulse = 2000;

    // 中立位置を計算（最小値と最大値の中間）
    m_neutral_pulse = (m_min_pulse + m_max_pulse) / 2;
}

ServoController::~ServoController() {

}

uint8_t ServoController::isInitialized() {

    return PwmController::isInitialized();
}

void ServoController::setPulseRange(uint32_t min_pulse, uint32_t max_pulse) {

    m_min_pulse = min_pulse;
    m_max_pulse = max_pulse;

    // パルス幅範囲が変更されたので、中立位置を再計算
    m_neutral_pulse = (m_min_pulse + m_max_pulse) / 2;
}

uint8_t ServoController::setAngle(float angle) {

    // 角度の範囲チェック
    if (angle < MIN_ANGLE || angle > MAX_ANGLE) {

        return 1;
    }

    // 角度をパルス幅に変換
    uint32_t pulse_width_us = angleToParseWidth(angle);

    // パルス幅を設定
    return setPulseWidth(pulse_width_us);
}

uint8_t ServoController::neutral() {

    // 中立位置（0度）に設定：計算された中立パルス幅を使用
    return setPulseWidth(m_neutral_pulse);
}

uint8_t ServoController::stop() {

    return PwmController::stop();
}

uint32_t ServoController::angleToParseWidth(float angle) {

    // 角度をパルス幅に変換
    // 角度を0～180にスケール
    float angle_0_to_180 = angle + 90.0f;

    // パルス幅を計算（メンバー変数を使用）
    uint32_t pulse_range = m_max_pulse - m_min_pulse;
    uint32_t pulse_width_us = (uint32_t)(m_min_pulse + (angle_0_to_180 / 180.0f) * pulse_range);

    return pulse_width_us;
}
