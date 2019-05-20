#include "testing_core.hxx"
//unique integration test
TEST(is_homework_correct, is_homework_correct){
    bool mustBeTrue = is_homework_correct();
    ASSERT_TRUE(mustBeTrue);
}
