#include <bits/stdc++.h>
using namespace std;
#include "state_type.hpp"
#include "wrapper.hpp"
#include <stdio.h>
#include "PID.h"

PID roll_pid(0.8f, 0.1f, 0.05f, 0.01f);
PID pitch_pid(0.8f, 0.1f, 0.05f, 0.01f);
PID yaw_pid(0.5f, 0.05f, 0.01f, 0.01f);

void startf(State* current_state, Context* context){
	roll_pid.setLimit(10.0f, 5.0f);
	pitch_pid.setLimit(10.0f, 5.0f);
	yaw_pid.setLimit(10.0f, 5.0f);
    context->count++;
    printf("Start %d\n",context->count);
    *current_state = State::Init;
}
