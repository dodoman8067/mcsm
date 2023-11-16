#include <mcsm/util/algorithm_utils.h>

template<typename T>
void mcsm::compare(std::vector<T>& v1, std::vector<T>& v2){
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    return v1 == v2;
}
