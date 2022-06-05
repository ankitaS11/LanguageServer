#include <map>
#include <string>
#include <iostream>

struct Math
{
    double sinFunc(double x) { return 0.33; };
    double cosFunc(double x) { return 0.66; };
};

typedef double (Math::*math_method_t)(double);
typedef std::map<std::string, math_method_t> math_func_map_t;

int main()
{

    math_func_map_t mapping;
    mapping["sin"] = &Math::sinFunc;
    mapping["cos"] = &Math::cosFunc;

    std::string function = std::string("sin");
    math_func_map_t::iterator x = mapping.find(function);
    int result = 0;

    if (x != mapping.end()) {
        Math m;
        result = int(m.*(x->second));
        std::cout << result;
    }
}
