#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
#include <string>

using namespace std;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

int main(int argc, char** argv) {

    auto pFile = fopen ( "country.wav" , "rb" );
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


    curl_global_init(CURL_GLOBAL_DEFAULT);
    auto curl = curl_easy_init();
    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5000/predict");
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

        /* Create the form */
        auto form = curl_mime_init(curl);
    
        /* Fill in the file upload field */
        auto part = curl_mime_addpart(form);
        curl_mime_data(part, buffer, buffer_size);
        curl_mime_name(part, "file");
        curl_mime_filename(part, "audio.wav");

        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

        std::string response_string;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        curl_easy_perform(curl);
        cout << response_string;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        curl = NULL;
    }
}