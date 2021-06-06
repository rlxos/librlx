#include "../utils/exec.hh"

using namespace rlx::utils;

int main(int ac, char **av)
{
    return exec::command("echo $VARIABLE", ".", std::vector<std::string>{"VARIABLE=5"});
}