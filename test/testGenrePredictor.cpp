#include "../src/GenrePredictor.hpp""
#include <gtest/gtest.h>

// Tests zero-input.
TEST(getRGBTest, ReturnsConstructorValues){

    char* buffer;
    int buffer_size = 0;
    GenrePredictor predictor;
    auto predictions = predictor.predict(buffer, buffer_size);

    EXPECT_EQ(predictions.size(), 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
