#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ftw.h>
#include <zipper/unzipper.h>
#include "curse.h"

//#include "include/rapidjson/" //decided against using this for now

using namespace std;

//extern std::string getCurseAddonUrl(std::string addonName, bool WowVersion);



int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

int rmrf(const char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}



void downloadFile(string urlOfFile, string pathOfWow, string fName)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    const char *url = urlOfFile.c_str();
    //char *redurl = NULL;
    //char outfilename[FILENAME_MAX] = "test.txt";
    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(string(pathOfWow + fName).c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  //follow through redirects
        
        res = curl_easy_perform(curl);
        //curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &redurl); //for getting name of file.  fix later. 
        curl_easy_cleanup(curl);
        fclose(fp);
        /*
        //for finding file name.
        cout << "REDIR HERE##################" << endl;
        if(redurl)
            printf("%s\n", redurl);
        else
            cout << "Ruh Roh." << endl;
        */
    }
}

void downloadFileWowAce(string projectToDownload, string& wowAce, string& wowAddons)
{
    downloadFile(wowAce + projectToDownload + "/files/latest", wowAddons, projectToDownload + ".zip");
}

void loadConfig(string& wowAddons, vector<string>& names, bool& wowVersion)
{
    ifstream configFile;
    configFile.open("config.ini");
    getline(configFile, wowAddons);

    string tempC(wowAddons);
    tempC=tempC.back();
    if(tempC.compare("/")!=0)
        wowAddons = wowAddons + "/";


    getline(configFile, tempC);
    wowVersion = 0;
    if(tempC.compare("classic"))
        wowVersion = 1;
    //get vector of all addons
    string tempAddon;
    while(!configFile.eof())
    {
        getline(configFile, tempAddon);
        names.push_back(tempAddon);
    }

    configFile.close();
    cout << wowAddons << endl;
    if(rmrf(wowAddons.c_str())!=0)
    {
        cout << "Ruh roh, couldnt delete the directory..." << endl;
    }
    else
    {
        if (mkdir(wowAddons.c_str(), 0755) < 0) {
            if (errno != EEXIST) {
                perror(wowAddons.c_str());
            }
        }
    }
}


int main()
{
    string wowAce = "https://www.wowace.com/projects/";

    vector<string> addonNames;

    bool ver;

    string wowAddons; //, remoteAddon[256];
    cout << "Loading Config." << endl;
    loadConfig(wowAddons, addonNames, ver);

    for (vector<string>::iterator it = addonNames.begin(); it != addonNames.end(); ++it)
    {
        downloadFile(getCurseAddonUrl(*it, ver), wowAddons, *it + ".zip");
    }

    //downloadFile(getCurseAddonUrl(remoteAddon, 1), wowAddons, remoteAddon + ".zip");
    //downloadFileWowAce(remoteAddon, wowAce, wowAddons);
    //unzip(wowAddons+remoteAddon+".zip", wowAddons);
    for (vector<string>::iterator it = addonNames.begin(); it != addonNames.end(); ++it)
    {
        zipper::Unzipper unzupwow(wowAddons+ (*it)+".zip");
        unzupwow.extract(wowAddons);
        unzupwow.close();
        
    }
    return 0;
}