#include "simcc/misc/md5.h"
#include "simcc/random.h"
#include "simcc/timestamp.h"

namespace simcc {

string MD5::GenBinMD5(const size_t random_data_len) {
    thread_local static simcc::Random rand(uint32_t(Timestamp::Now().Unix()));
    string data;
    data.resize(random_data_len);

    for (size_t i = 0; i < random_data_len; ++i) {
        data[i] = rand.Next() % 255;
    }

    return MD5::Sum(reinterpret_cast<const void*>(data.data()), data.length());
}

string MD5::GenHexMD5(const size_t random_data_len) {
    string bin = GenBinMD5(random_data_len);
    return Bin2Hex(bin.data(), bin.size());
}

}


