#ifndef INC_SERVO_CONTROLLER_HPP_
#define INC_SERVO_CONTROLLER_HPP_

#include "pwm_controller.hpp"

class ServoController : public PwmController {

    public:

        // コンストラクタ
        ServoController(TIM_HandleTypeDef* htim, uint32_t channel);

        // デストラクタ
        ~ServoController();

        // 初期化状態の確認
        uint8_t isInitialized();

        // パルス幅の範囲を設定
        void setPulseRange(uint32_t min_pulse, uint32_t max_pulse);

        // 角度を設定（-90 ~ 90度）
        uint8_t setAngle(float angle);

        // 中立位置に移動（0度）
        uint8_t neutral();

        // 出力を停止
        uint8_t stop();

    private:

        // 角度をパルス幅に変換
        uint32_t angleToParseWidth(float angle);

        // 最小パルス幅（-90度）[us]
        uint32_t m_min_pulse;

        // 最大パルス幅（+90度）[us]
        uint32_t m_max_pulse;

        // 中立位置のパルス幅（0度）[us]
        uint32_t m_neutral_pulse;

        // 最小角度（度）
        static constexpr float MIN_ANGLE = -90.0f;

        // 最大角度（度）
        static constexpr float MAX_ANGLE = 90.0f;
};

#endif /* INC_SERVO_CONTROLLER_HPP_ */
