#include "curse.h"

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string getCurseAddonUrl(string addonName, bool WowVersion)
{
    cout << "GETTING CURSE ADDON PAGE" << endl;
    cout << "Addon Name: " << addonName << endl;
    cout << "WoW Version: " << WowVersion << endl << endl;
    


    string returnUrl = "DUNK";

    CURL* curlHandle;
    CURLcode res;
    string readBuffer;
    string subSearch;
    string urlToAccess = "https://www.curseforge.com/wow/addons/" + addonName;
    cout << "URL about to access: " << urlToAccess << endl; 

    curlHandle = curl_easy_init();
    
    if(curlHandle) {
        curl_easy_setopt(curlHandle, CURLOPT_URL, urlToAccess.c_str());
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt (curlHandle, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curlHandle, CURLOPT_HEADER, "Accept: text/html");
        curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64; rv:69.0) Gecko/20100101 Firefox/69.0");
        res = curl_easy_perform(curlHandle);
        curl_easy_cleanup(curlHandle);

        ofstream logFile;
        logFile.open("log.txt", ofstream::out | ofstream::trunc);
        logFile << readBuffer;
        logFile.close();

        //std::cout << readBuffer << std::endl;
        size_t foundName;

        if(WowVersion)
            foundName = readBuffer.find("WoW Classic\r\n");
        else
            foundName = readBuffer.find("WoW\r\n");
        

        if(foundName!=string::npos)
        {
            subSearch = readBuffer.substr(foundName, string::npos);
            cout << subSearch << endl << "END OF FIRST ITERATION" << endl;
            foundName = subSearch.find("/files/");
            subSearch = subSearch.substr(foundName + 7, string::npos);
            cout << subSearch << endl << "END OF SECOND ITERATION" << endl;
            foundName = subSearch.find("\"");
            returnUrl = subSearch.substr(0, foundName);

            returnUrl = "https://www.curseforge.com/wow/addons/" + addonName + "/download/" + returnUrl + "/file";
        }
    }
    cout << "CURL ERROR" << endl;
    cout << curl_easy_strerror(res) << endl;
    cout << "Printing URL:";
    cout << returnUrl << endl;
    return returnUrl;
}