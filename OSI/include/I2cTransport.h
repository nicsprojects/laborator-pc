#ifndef I2C_TRANSPORT_H
#define I2C_TRANSPORT_H

#include "IAdxlTransport.h"

class I2cTransport : public IAdxlTransport {
public:
    void writeRegister(char reg, char value) override;
    char readRegister(char reg) override;
};

#endif