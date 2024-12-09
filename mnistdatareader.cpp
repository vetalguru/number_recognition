// Copyright 2024 Vitalii Shkibtan

#include "./mnistdatareader.h"

#include <algorithm>
#include <cassert>
#include <sstream>

#include "./mnistdataitem.h"

bool MnistDataReader::open(const std::string& path) noexcept {
    if (path.empty()) {
        return false;
    }

    m_file.open(path, std::fstream::in);
    return m_file.good();
}

void MnistDataReader::close() noexcept {
    if (isOpened()) {
        m_file.close();
    }
}

bool MnistDataReader::isOpened() const noexcept {
    return m_file.is_open();
}

bool MnistDataReader::dataItem(int index, MnistDataItem& item) noexcept {
    std::string line;
    if (!readDataLine(index, line)) {
        return false;
    }

    normalizeDataString(line);

    return parseMnistLine(line, item);
}

bool MnistDataReader::readDataLine(const int index,
        std::string &line) noexcept {
    line.clear();

    std::string cur;
    int lineCounter = 0;
    while (std::getline(m_file, cur)) {
        if (lineCounter++ == index) {
            line = cur;
            break;
        }
    }

    return !line.empty();
}

bool MnistDataReader::parseMnistLine(const std::string& line,
                                     MnistDataItem& item) const noexcept {
    item.clear();

    if (line.empty()) {
        return false;
    }

    std::stringstream ss(line);
    const char delimiter = ',';
    std::string temp;
    int i = 0;
    while (std::getline(ss, temp, delimiter)) {
        assert(i <= item.size());

        try {
            item[i++] = stoi(temp);
        } catch (...) {
            return false;
        }
    }

    return true;
}

void MnistDataReader::normalizeDataString(std::string &line) const noexcept {
    const char chars[] = {'\n', ' '};

    for (const auto &ch : chars) {
        line.erase(std::remove(line.begin(), line.end(), ch), line.cend());
    }
}
