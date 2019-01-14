#include "machine.h"

void interpretor::machine::increase_pc(){
    ++(m_register_pc.data.m_number);
}

void interpretor::machine::print(char* message)
{
    printer.log(message);
}

void interpretor::machine::set_run_error() {//need modify
    m_error_flag = 1;
}

void interpretor::machine::set_ret_register(value val) {
    m_register_ret = val;
}