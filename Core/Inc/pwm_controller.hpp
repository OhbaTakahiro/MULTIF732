#ifndef INC_PWM_CONTROLLER_HPP_
#define INC_PWM_CONTROLLER_HPP_

#include "tim.h"

class PwmController {

    public:

        // コンストラクタ（タイマーハンドルとチャンネルを指定）
        PwmController(TIM_HandleTypeDef* htim, uint32_t channel);

        // デストラクタ
        ~PwmController();

        // 初期化状態の確認
        uint8_t isInitialized();

        // パルス幅を設定
        uint8_t setPulseWidth(uint32_t pulse_width_us);

        // PWM出力を停止
        uint8_t stop();

    private:

        // パルス幅の範囲チェック
        bool checkPulseWidthRange(uint32_t pulse_width_us);

        // タイマーハンドルへのポインタ
        TIM_HandleTypeDef* m_htim;

        // 使用するチャンネル
        uint32_t m_channel;

        // 初期化フラグ
        uint8_t m_is_initialized;

        // タイマーのクロック周波数（Hz）
        uint32_t m_timer_clock;

        // タイマープリスケーラー
        uint32_t m_prescaler;

        // タイマー周波数（Hz）
        uint32_t m_timer_freq;

        // パルス幅の最小値（us）
        static constexpr uint32_t MIN_PULSE_WIDTH = 1000;

        // パルス幅の最大値（us）
        static constexpr uint32_t MAX_PULSE_WIDTH = 2000;
};

#endif /* INC_PWM_CONTROLLER_HPP_ */
