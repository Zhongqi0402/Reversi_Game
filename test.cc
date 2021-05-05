#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> vec;
    std::vector<int> v;
    for(int j = 0; j < 6; j += 1) {
        for(int i = 0; i < 5; i += 1) {
            v.emplace_back(i);
        }
        vec.emplace_back(v);
        v.clear();
    }
    std::cout << vec.size() << " " << vec[1].size() << std::endl;
    

}