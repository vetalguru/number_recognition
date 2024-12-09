// Copyright 2024 Vitalii Shkibtan

#ifndef MNISTDATAREADER_H_
#define MNISTDATAREADER_H_

#include <fstream>
#include <string>

class MnistDataItem;

class MnistDataReader {
    public:
        MnistDataReader() = default;
        ~MnistDataReader() = default;

        MnistDataReader(const MnistDataReader&) = delete;
        MnistDataReader(MnistDataReader&&) = delete;

        MnistDataReader& operator = (const MnistDataReader&) = delete;
        MnistDataReader& operator = (MnistDataReader&&) = delete;

        bool open(const std::string& path) noexcept;
        void close() noexcept;
        bool isOpened() const noexcept;
        bool dataItem(int index, MnistDataItem& item) noexcept;

    private:
        bool readDataLine(const int index, std::string &line) noexcept;
        bool parseMnistLine(const std::string& line, MnistDataItem& item) const noexcept;
        void normalizeDataString(std::string &line) const noexcept;

    private:
        std::fstream m_file;
};

#endif  // MNISTDATAREADER_H_
