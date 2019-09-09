#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string getCurseAddonUrl(string addonName, bool WowVersion); //WowVersion: 0 for retail, 1 for classic