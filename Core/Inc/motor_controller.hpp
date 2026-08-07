#ifndef INC_MOTOR_CONTROLLER_HPP_
#define INC_MOTOR_CONTROLLER_HPP_

#include "pwm_controller.hpp"

// モーターの動作モードを定義
enum class MotorMode : uint8_t {

    NORMAL = 0,    // 通常モード（0 ~ 100%）
    REVERSE = 1    // リバースモード（-100 ~ 100%）
};

class MotorController : public PwmController {

    public:

        // コンストラクタ（モード設定）
        MotorController(TIM_HandleTypeDef* htim, uint32_t channel, MotorMode mode = MotorMode::NORMAL);

        // デストラクタ
        ~MotorController();

        // 初期化状態の確認
        uint8_t isInitialized();

        // パルス幅の範囲を設定
        void setPulseRange(uint32_t min_pulse, uint32_t max_pulse);

        // 速度を設定（通常モード: 0 ~ 100 %、リバースモード: -100 ~ 100 %）
        uint8_t setSpeed(float speed_percent);

        // 出力を停止
        uint8_t stop();

    private:

        // 速度（%）をパルス幅に変換
        uint32_t speedToParseWidth(float speed_percent);

        // 最小パルス幅（0%）[us]
        uint32_t m_min_pulse;

        // 最大パルス幅（100%）[us]
        uint32_t m_max_pulse;

        // ニュートラル位置のパルス幅[us]
        uint32_t m_neutral_pulse;

        // リバースモード設定（0: 通常モード、1: リバースモード）
        MotorMode m_mode;

        // 通常モードの最小速度（%）
        static constexpr float MIN_SPEED = 0.0f;

        // 通常モードの最大速度（%）
        static constexpr float MAX_SPEED = 100.0f;

        // リバースモードの最小速度（%）
        static constexpr float REVERSE_MIN_SPEED = -100.0f;

        // リバースモードの最大速度（%）
        static constexpr float REVERSE_MAX_SPEED = 100.0f;
};

#endif /* INC_MOTOR_CONTROLLER_HPP_ */
