#include <curl/curl.h>

#include <filesystem>
#include <iostream>

#include <math.h>

#include "curl.hh"
#include "../utils/progressbar.hh"
#include "../utils/string.hh"

#include <assert.h>
#include <sys/stat.h>
#include <libgen.h>

namespace rlx::curl
{

    utils::progressbar *__progress = nullptr;
    static float already_done = 0;

    static int progress_func(void *ptr, double TotalToDownload, double NowDownloaded,
                             double TotalToUpload, double NowUploaded)
    {
        assert(__progress != nullptr);
        NowDownloaded += already_done;
        TotalToDownload += already_done;

        __progress->mesg(utils::string::humanize(NowDownloaded) + "/" + utils::string::humanize(TotalToDownload));
        __progress->update(std::cout, static_cast<float>((NowDownloaded / TotalToDownload) * 100.0));

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

        file = fopen((path + ".tmp").c_str(), "a+b");
        if (!file)
        {
            std::cout << "failed to open " << path << " for write" << std::endl;
            return false;
        }

        struct stat st = {0};
        if (!stat((path + ".tmp").c_str(), &st))
            ;

        already_done = st.st_size;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM, st.st_size);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        __progress = new utils::progressbar();

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1000);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 10);

        auto out = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_cleanup(curl);
        fclose(file);

        io::println("\r");

        if (out == CURLE_OK && http_code == 200)
            std::filesystem::rename(path + ".tmp", path);

        delete __progress;
        return out == CURLE_OK && http_code == 200;
    }
}