#ifndef INSTRUMENTS_PROTOCOL_H
#define INSTRUMENTS_PROTOCOL_H

#include <string>
#include "global.h"

class NOT_EXPORT InstrumentProtocol
{
public:
    InstrumentProtocol();
    ~InstrumentProtocol();

    void setInstrument(Instrument id);

    void setDof1(uint16_t v);
    void setDof2(uint16_t v);
    void setDof3(uint16_t v);
    void setDof4(uint16_t v);

    void setData(size_t index, char value);

    const std::string& get();

private:
    std::string data_;
};

#endif // INSTRUMENTS_PROTOCOL_H
