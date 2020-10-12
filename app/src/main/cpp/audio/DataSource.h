//
// Created by Ademir Queiroga on 05/05/21.
//

#ifndef MAESTRO_DATASOURCE_H
#define MAESTRO_DATASOURCE_H

#include <cstdint>
#include "../Constants.h"

class DataSource {
public:
    virtual ~DataSource(){};
    virtual int64_t getSize() const = 0;
    virtual AudioProperties getProperties() const = 0;
    virtual const float* getData() const = 0;
};


#endif //MAESTRO_DATASOURCE_H
