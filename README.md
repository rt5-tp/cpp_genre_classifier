# Genre Classifier CPP

The GenreClassifier is a client for the GenrePredictionServer and allows for the classification of any given music into one of 10 genres, given a .wav audio buffer. 
Predictions are in the form of pairs, ie. genre (string) -> certainty (float) 

## Dependencies

- [libcurl](https://curl.se/libcurl/)
- local genre predictor server (link to be added...)

## Building

To build, enter the src directory and run following commands:

> cmake . <br>
> make

## Running

To run, run following command:

> ./GenreClassifier