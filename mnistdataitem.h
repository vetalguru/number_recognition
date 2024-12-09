// Copyright 2024 Vitalii Shkibtan

#ifndef MNISTDATAITEM_H_
#define MNISTDATAITEM_H_

#include <memory>

class MnistDataItem {
public:
    const int kWidth  = 28;
    const int kHeight = 28;
    const int kLength = kWidth * kHeight + 1; // bitmap and supposed value
    const int kWrongValue = -1;

    MnistDataItem() = default;
    ~MnistDataItem() = default;

    MnistDataItem(const MnistDataItem&) = delete;
    MnistDataItem(MnistDataItem&&) = delete;
                
    MnistDataItem& operator=(const MnistDataItem&) = delete;
    MnistDataItem& operator=(MnistDataItem&&) = delete;

    size_t size() const noexcept;
    void clear() noexcept;

    int supposed() const noexcept;
    int& operator[](int index) const noexcept;

private:
    std::unique_ptr<int[]> m_data{new int[kLength]{0}};
};

#endif  // MNISTDATAITEM_H_
