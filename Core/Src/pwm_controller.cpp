#include "pwm_controller.hpp"

PwmController::PwmController(TIM_HandleTypeDef* htim, uint32_t channel)
    : m_htim(htim), m_channel(channel), m_is_initialized(0), m_timer_clock(0), m_prescaler(0), m_timer_freq(0) {

    // タイマーハンドルが有効か確認
    if (m_htim == nullptr) {

        m_is_initialized = 0;
        return;
    }

    // タイマーのクロック周波数を取得
    // STM32F446の場合、タイマークロック = APB1/APB2ペリフェラルクロック × 2
    // APB1タイマークロック（90MHz）を取得
    uint32_t apb1_clock = HAL_RCC_GetPCLK1Freq();
    m_timer_clock = apb1_clock * 2;

    // タイマープリスケーラーを取得
    m_prescaler = m_htim->Init.Prescaler;

    // タイマー周波数を計算
    m_timer_freq = m_timer_clock / (m_prescaler + 1);

    // 初期化成功
    m_is_initialized = 1;

    // PWM出力を開始
    HAL_TIM_PWM_Start(m_htim, m_channel);
}

PwmController::~PwmController() {

    // タイマーを停止
    if (m_htim != nullptr) {

        HAL_TIM_PWM_Stop(m_htim, m_channel);
    }
}

uint8_t PwmController::isInitialized() {

    return m_is_initialized;
}

uint8_t PwmController::setPulseWidth(uint32_t pulse_width_us) {

    // 初期化チェック
    if (!m_is_initialized) {

        return 1;
    }

    // パルス幅の範囲チェック
    if (!checkPulseWidthRange(pulse_width_us)) {

        return 1;
    }

    // パルス幅（us）をカウント値に変換
    uint32_t pulse_count = (pulse_width_us * m_timer_freq) / 1000000;

    // Compare値を設定
    __HAL_TIM_SET_COMPARE(m_htim, m_channel, pulse_count);

    return 0;
}

uint8_t PwmController::stop() {

    // 初期化チェック
    if (!m_is_initialized) {

        return 1;
    }

    // PWM出力を停止
    if (HAL_TIM_PWM_Stop(m_htim, m_channel) != HAL_OK) {

        return 1;
    }

    return 0;
}

bool PwmController::checkPulseWidthRange(uint32_t pulse_width_us) {

    // パルス幅が範囲内かチェック
    if (pulse_width_us < MIN_PULSE_WIDTH || pulse_width_us > MAX_PULSE_WIDTH) {

        return false;
    }

    return true;
}
