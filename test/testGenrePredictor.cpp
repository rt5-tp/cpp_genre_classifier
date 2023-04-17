#include "../src/GenrePredictor.hpp""
#include <gtest/gtest.h>

// Tests zero-input.
TEST(predictTest, ReturnsEmptyOnEmptyBuffer){

    char* buffer;
    int buffer_size = 0;
    GenrePredictor predictor;
    auto predictions = predictor.predict(buffer, buffer_size);

    // check there's no response if our buffer is empty
    EXPECT_EQ(predictions.size(), 0);
}

// Tests song genre prediction
TEST(predictTest, ReturnsPredictionOnFullBuffer){

    auto pFile = fopen ( "country-1p5sec.wav" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    auto buffer_size = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    char* buffer = (char*) malloc (sizeof(char)*buffer_size);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    auto result = fread (buffer,1,buffer_size,pFile);
    if (result != buffer_size) {fputs ("Reading error",stderr); exit (3);}

    GenrePredictor predictor;
    auto predictions = predictor.predict(buffer, buffer_size);

    free(buffer);

    // check we have 10 predictions returned
    EXPECT_EQ(predictions.size(), 10);

    std::set<std::string> expected_genres {"blues", "classical", "country","disco","hiphop","jazz","metal","pop","reggae","rock"};

    for(auto prediction : predictions){

        // check correct prediction is returned
        if(prediction.first == "country") EXPECT_EQ(prediction.second, 1.0);

        // check the genres are as expected
        EXPECT_NE(expected_genres.find(prediction.first), expected_genres.end());
    }

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
