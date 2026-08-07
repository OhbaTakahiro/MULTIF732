#include <bits/stdc++.h>
using namespace std;
#include "state_type.hpp"
#include "wrapper.hpp"
#include <stdio.h>

void calibf(State* current_state, Context* context){
    context->count++;
    printf("Calib %d\n",context->count);
    *current_state = State::Prearm;
}
