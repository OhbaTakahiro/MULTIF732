#include "pwm_controller.hpp"

PwmController::PwmController(TIM_HandleTypeDef* htim, uint32_t channel)
    : m_htim(htim), m_channel(channel), m_is_initialized(0), m_timer_clock(0), m_prescaler(0), m_timer_freq(0) {

    // タイマーハンドルが有効か確認
    if (m_htim == nullptr) {

        m_is_initialized = 0;
        return;
    }

    // TIM1/TIM8 は APB2、それ以外は APB1 に接続される。APB 分周が
    // 1 以外のときだけ、タイマークロックはペリフェラルクロックの 2 倍となる。
    const bool is_apb2_timer = (m_htim->Instance == TIM1) || (m_htim->Instance == TIM8);
    const uint32_t apb_prescaler = is_apb2_timer
        ? (RCC->CFGR & RCC_CFGR_PPRE2)
        : (RCC->CFGR & RCC_CFGR_PPRE1);

    m_timer_clock = is_apb2_timer ? HAL_RCC_GetPCLK2Freq() : HAL_RCC_GetPCLK1Freq();
    if (apb_prescaler != RCC_HCLK_DIV1) {
        m_timer_clock *= 2U;
    }

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

    // ESC のディスアームは PWM を止めず、最小パルスを出し続ける。
    // これにより再アーム時に PWM を開始し忘れることがない。
    return setPulseWidth(MIN_PULSE_WIDTH);
}

bool PwmController::checkPulseWidthRange(uint32_t pulse_width_us) {

    // パルス幅が範囲内かチェック
    if (pulse_width_us < MIN_PULSE_WIDTH || pulse_width_us > MAX_PULSE_WIDTH) {

        return false;
    }

    return true;
}
