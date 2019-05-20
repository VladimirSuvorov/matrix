#include "testing_core.hxx"

TEST(is_homework_correct, is_homework_correct){
    using namespace std;
    bool mustBeTrue = is_homework_correct();
    ASSERT_TRUE(mustBeTrue);
}
