#include <curl/curl.h>

#include <filesystem>
#include <iostream>

#include <math.h>

#include "curl.hh"

namespace rlx::curl
{

    int progress_func(void *ptr, double TotalToDownload, double NowDownloaded,
                      double TotalToUpload, double NowUploaded)
    {
        // ensure that the file to be downloaded is not empty
        // because that would cause a division by zero error later on
        if (TotalToDownload <= 0.0)
        {
            return 0;
        }

        // how wide you want the progress meter to be
        int totaldotz = 40;
        double fractiondownloaded = NowDownloaded / TotalToDownload;
        // part of the progressmeter that's already "full"
        int dotz = (int)round(fractiondownloaded * totaldotz);

        // create the "meter"
        int ii = 0;
        printf("%3.0f%% [", fractiondownloaded * 100);
        // part  that's full already
        for (; ii < dotz; ii++)
        {
            printf("=");
        }
        // remaining part (spaces)
        for (; ii < totaldotz; ii++)
        {
            printf(" ");
        }
        // and back to line begin - do not forget the fflush to avoid output buffering problems!
        printf("]\r");
        fflush(stdout);
        // if you don't return 0, the transfer will be aborted - see the documentation
        return 0;
    }

    size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        size_t written = fwrite(ptr, size, nmemb, stream);
        return written;
    }

    bool download(std::string url, std::string path)
    {
        if (std::filesystem::exists(path))
            return true;

        CURL *curl;
        FILE *file;

        curl = curl_easy_init();
        if (!curl)
        {
            std::cout << "failed to init curl" << std::endl;
            return false;
        }

        file = fopen((path + ".tmp").c_str(), "wb");
        if (!file)
        {
            std::cout << "failed to open " << path << " for write" << std::endl;
            return false;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        auto out = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(file);
        if (out == CURLE_OK)
            std::filesystem::rename(path + ".tmp", path);

        return out == CURLE_OK;
    }
}