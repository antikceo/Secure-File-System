#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

vector<string> validusers(string filename){
  string line;
  int count = 0;
  vector<string> valid;
  ifstream iFile(filename.c_str());

  while(getline(iFile, line))
    {
      valid.push_back(split(line, ' ').front());
      //cout << "This is:" << valid.at(count) << " " << count << "\n";                                                                                                             
      count++;
    }

  return valid;
}

bool isValidUser(vector<string> validUsers, string thisUser){
  int checker = 0;
  int length = validUsers.size();

  for (int i = 0; i < length; i++){
    if(validUsers.at(i) == thisUser)
      checker = 1;
  }
  if (checker == 0){
    return false;
  }else{
    return true;
  }
}

vector<string> split(string str, const char delim) {
  vector<string> v;
  string tmp;
  string::iterator i;

  for(i = str.begin(); i <= str.end(); ++i) {
    if((const char)*i != delim  && i != str.end()) {
      tmp += *i;
    } else {
      v.push_back(tmp);
      tmp = "";
    }
  }
  return v;
}

