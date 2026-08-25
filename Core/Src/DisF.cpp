#include "state_type.hpp"
#include "wrapper.hpp"
#include <stdio.h>

void disf(State* current_state, Context* context){
    context->count++;
    *current_state = State::Prearm;
}
