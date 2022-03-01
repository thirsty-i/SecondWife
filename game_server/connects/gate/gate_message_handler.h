#include "common/noncopyable.h"

class gate_message_handler final
    : private  noncopyable
{
public:
    using pointer = std::shared_ptr<gate_message_handler>;

private:
};
