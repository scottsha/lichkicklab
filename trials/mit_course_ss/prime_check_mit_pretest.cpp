#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>

bool is_prime (int x){
    int upper_bound = std::sqrt(x);
    for (int y=2; y<=upper_bound; y++){
        if (x%y==0) return false;
    }
    return true;
}

std::string say_is_prime(bool x){
    if (x) return " is prime. ";
    return " is composite. ";
}

void problem_8(){
    for (auto foo=2; foo<100; foo++) std::cout << foo << say_is_prime(is_prime(foo));
}

std::vector<int> nike_swoosh_shape(std::vector<int> x){
    auto it = x.begin();
    auto lead = *it;
    std::sort(x.begin(), x.end());
    while(it!=x.end()){
        it++;
        if (*it>lead) break;
    }
    auto newlead = *it;
    std::vector<int> y(x.begin(), x.end());
    std::copy(x.begin(), it, y.begin()+1);
    y.at(0) = newlead;
    return y;
}


bool next_permutation_inplace(std::vector<int>& x){
    //Finds the next permutation in lexical order and returns false if the last one.
    if (x.size()<2){
        return false;
    }
    auto it = x.end()-1;
    while (it != x.begin()){
        auto at = *it;
        it--;
        auto prev = *it;
        if (prev < at){
            std::vector<int> nikem(it, x.end());
            auto niked = nike_swoosh_shape(nikem);
            std::copy(niked.begin(), niked.end(), it);
            return true;
        }
    }
    return false;
}

void print_vector(std::vector<int> x){
    for (auto foo=x.begin(); foo!=x.end()-1; foo++){
        std::cout << *foo << " ";
    }
    std::cout << *(x.end()-1) << std::endl;
}


void print_permutations_lexically(int n){
    std::vector<int> roger(n);
    for (auto foo=0; foo<n; foo++) roger[foo] = foo+1;
    print_vector(roger);
    while(next_permutation_inplace(roger)){
        print_vector(roger);
    }
}


void problem_9(){
    print_permutations_lexically(4);
}

int direct_search_linear_time(int target, std::vector<int> haystack){
    for (auto x=0; x<haystack.size(); x++) if (target==haystack.at(x)) return x;
    return haystack.size();
}

int binary_search_sorted(int target, std::vector<int> haystack){
    if (haystack.empty()) return false;
    int width = static_cast<int>(haystack.size());
    if (*(haystack.end()-1) == target) return width-1;
    int atfront = 0;
    while (width>0){
        if (width%2==0){
            width/=2;
        }
        else{ width--; }
        int mid = atfront + width;
        int check = haystack.at(mid);
        if (check==target) return mid;
        if (check < target){
            atfront = mid;
        }
    }
    return haystack.size();
}

void problem_10(){
    std::vector<int> xax{1,2,5,8,9,11,12,14,16,17,18,19,20};
    for(auto foo=5; foo<21; foo++){
        auto actual = static_cast<int>(std::find(xax.begin(),xax.end(), foo) - xax.begin());
        int lin = direct_search_linear_time(foo, xax);
        int bin = binary_search_sorted(foo, xax);
        std::cout << "I see " << foo << " "<< actual << " " << lin  << " " << bin << std::endl;
    }
}

int main() {
    problem_8();
    std::cout << std::endl;
    problem_9();
    std::cout << std::endl;
    problem_10();
}
