#include <bits/stdc++.h>
using namespace std;
#include "state_type.hpp"
#include "wrapper.hpp"
#include <stdio.h>

void armf(State* current_state, Context* context, int sbusdata9ch){
    context->count++;
    printf("Arm %d\n",context->count);
    if(sbusdata9ch < 1000){
    	*current_state = State::Dis;
    }
    *current_state = State::Fly;
}
