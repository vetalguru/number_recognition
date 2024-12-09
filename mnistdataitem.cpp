// Copyright 2024 Vitalii Shkibtan

#include "mnistdataitem.h"

#include <algorithm>
#include <cassert>

size_t MnistDataItem::size() const noexcept {
    return kLength;
}

void MnistDataItem::clear() noexcept {
    std::fill_n(m_data.get(), kLength, kWrongValue);
}

int MnistDataItem::supposed() const noexcept {
    return m_data[0];
}

int& MnistDataItem::operator[](int index) const noexcept {
    assert((index >= 0) && (index < size()));

    return m_data[index];
}
