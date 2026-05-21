#ifndef SPI_TRANSPORT_H
#define SPI_TRANSPORT_H

#include "IAdxlTransport.h"

class SpiTransport : public IAdxlTransport {
public:
    void writeRegister(char reg, char value) override;
    char readRegister(char reg) override;
};

#endif