#pragma once

void startf(State* current_state, Context* context);
void initf(State* current_state, Context* context);
void calibf(State* current_state, Context* context);
void prearmf(State* current_state, Context* context, int sbusdata9ch);
void armf(State* current_state, Context* context, int sbusdata9ch);
void flyf(State* current_state, Context* context, int sbusdata9ch, int sbusdata3ch, int sbusdata1ch, int sbusdata4ch, int sbusdata2ch);
void disf(State* current_state, Context* context);
