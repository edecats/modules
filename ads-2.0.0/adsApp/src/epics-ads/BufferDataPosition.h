/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#ifndef BUFFERDATAPOSITION_H
#define BUFFERDATAPOSITION_H

#include <memory>
#include "SumReadBuffer.h"

struct BufferDataPosition {
    SumReadBuffer *buffer;
    uint32_t off_result;
    size_t off_data;

    bool operator == (const BufferDataPosition& br) const {
        return (this->buffer == br.buffer) && (this->off_result == br.off_result) && (this->off_data == br.off_data);
    }

    bool operator != (const BufferDataPosition& br) const {
        return (this->buffer != br.buffer) || (this->off_result != br.off_result) || (this->off_data != br.off_data);
    }
};

const BufferDataPosition EMPTY_BUFFER_DATA_POSITION = {nullptr, 0, 0};

#endif /* BUFFERDATAPOSITION_H */
