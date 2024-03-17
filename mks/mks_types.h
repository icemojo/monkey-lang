#ifndef MKS_TYPES_
#define MKS_TYPES_

#include <string>
#include <stdexcept>


namespace mks 
{

struct stoi_result {
    bool success{ false };
    int32_t value{ 0 };
};

[[nodiscard]]
stoi_result stoi(const std::string &str)
{
    stoi_result result{
        .success = false,
        .value = 0,
    };

    int32_t value{ 0 };
    try {
        value = std::stoi(str);
    }
    catch (std::invalid_argument const&) {
        return result;
    }
    catch (std::out_of_range const&) {
        // TODO(yemon): Might wanna upscale and try again with stoll(..)
        return result;
    }

    result.value = value;
    return result;
}

struct stoll_result {
    bool success{ false };
    int64_t value{ 0 };
};

[[nodiscard]]
stoll_result stoll(const std::string &str)
{
    stoll_result result{
        .success = false,
        .value = 0,
    };

    int64_t value{ 0 };
    try {
        value = std::stoll(str);
    }
    catch (std::invalid_argument const&) {
        return result;
    }
    catch (std::out_of_range const&) {
        return result;
    }

    result.value = value;
    return result;
}

}

#endif  // MKS_TYPES_
