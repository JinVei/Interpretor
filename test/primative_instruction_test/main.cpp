#include "plog.h"
#include "machine.h"
#include <iostream>
using namespace interpretor;

class printer : public interpretor::logger {
    void log(const char* message) override {
        std::cout << message << std::endl;
    }
};

std::vector<instruction> repertoire = {
    { operator_type::ADDITION,
      std::list<operand>{ { operand_type::immediate_operand,
                            value(10),
                          },
                          { operand_type::immediate_operand,
                            value(20),
                          }
                        }
    },
    { operator_type::SUBTRACTION,
      std::list<operand>{ { operand_type::register_operand,
                              register_to_value(register_t::ret)
                            },
                            { operand_type::immediate_operand,
                              value(6)
                            }
                          }
    },
    { operator_type::MULTIPLICATION,
      std::list<operand>{ { operand_type::register_operand,
                              register_to_value(register_t::ret)
                            },
                            { operand_type::immediate_operand,
                              value(2)
                            }
                          }
    },
    { operator_type::DIVISION,
      std::list<operand>{ { operand_type::register_operand,
                              register_to_value(register_t::ret)
                            },
                            { operand_type::immediate_operand,
                              value(3)
                            }
                          }
    },
    { operator_type::CMP,
      std::list<operand>{ { operand_type::register_operand,
                              register_to_value(register_t::ret)
                            },
                            { operand_type::immediate_operand,
                              value(16)
                            }
                          }
    },
    { operator_type::BRANCH,
      std::list<operand>{
                            { operand_type::immediate_operand,
                              value(1)
                            }
                          }
    },
    { operator_type::TABLE,
      std::list<operand>{
                            { operand_type::immediate_operand,
                              value("key1")
                            },
                            { operand_type::immediate_operand,
                              value("val1")
                            },
                            { operand_type::immediate_operand,
                              value("key2")
                            },
                            { operand_type::immediate_operand,
                              value("val2")
                            }
                          }
    },
    { operator_type::PUSH,
      std::list<operand>{
                           { operand_type::register_operand,
                             register_to_value(register_t::ret)
                           }
                         }
    },
    { operator_type::TABLE,
      std::list<operand>{
                         }
    },
    { operator_type::PUSH,
      std::list<operand>{
                           { operand_type::register_operand,
                             register_to_value(register_t::ret)
                           }
                         }
    },
    { operator_type::MOV,
      std::list<operand>{
                           { operand_type::stack_index_operand,
                             value(2),
                             register_to_value(register_t::ebp)
                           },
                           { operand_type::stack_index_operand,
                             value(1),
                             register_to_value(register_t::ebp),
                           }
                         }
    },
    { operator_type::TABLE_GET,
      std::list<operand>{
                           { operand_type::stack_operand,
                             value(1),
                             register_to_value(register_t::ebp)
                           },
                           { operand_type::immediate_operand,
                              value("key1")
                           }
                         }
    },
    { operator_type::TABLE_PUT,
      std::list<operand>{
                           { operand_type::stack_operand,
                             value(1),
                             register_to_value(register_t::ebp)
                           },
                           { operand_type::register_operand,
                             register_to_value(register_t::ret)
                           },
                           { operand_type::immediate_operand,
                             value("val3")
                           }
                         }
    },
    { operator_type::POP,
      std::list<operand>{
                         }
    },
    { operator_type::POP,
      std::list<operand>{
                         }
    },
};

void main() {
    Plog("");
    printer logger;
    interpretor::machine executor(&logger);
    executor.set_repertoire(std::vector<instruction>(repertoire));
    executor.run();
}