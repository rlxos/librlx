#include <rlx.hh>
#include <unittest/unittest.hh>

void test_sum()
{
    assert(5 == 5, "Addition");
}

int main(int ac, char **av)
{
    using namespace rlx;

    unittest::add(test_sum);

    unittest::run();
}