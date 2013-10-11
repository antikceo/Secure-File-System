#define _OPEN_SYS
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>
#include <iostream>
#include <iterator>
#include <pwd.h>

using namespace std;

void copyFile (string input, string output);
vector<string> userGroups(string username);
vector<string> split(string str, const char delim);
vector<string> validusers(string filename);
bool isValidUser(vector<string> validUsers, string thisUser);
bool canReadFile(string filename, string username);
bool canWriteFile(string filename, string username);
bool canExecuteFile(string filename, string username);
bool canReadAcl(string filename, string username);
bool canWriteAcl(string filename, string username);
bool contains(vector<string> collection, string loner); 
bool canDoSomething(string filename, string username, const char perm);
bool canReadiFile(string filename);

//checks if file exist or is readable
bool canReadiFile(string filename){
  ifstream input_data;
  input_data.open(filename.c_str());
  return input_data;
} 

//copies input file to output
void copyFile (string input, string output){
  fstream f(input.c_str(), fstream::in|fstream::binary);
  f << noskipws;
  istream_iterator<unsigned char> begin(f);
  istream_iterator<unsigned char> end;

  fstream f2(output.c_str(),
	     fstream::out|fstream::trunc|fstream::binary);
  ostream_iterator<char> begin2(f2);

  copy(begin, end, begin2);
}

//gets a list of groups a user belongs to by looking through the userfile.txt
int userGroups(){
  struct passwd *p;
  uid_t uid;
  int groupid;
  
  if ((p = getpwuid(uid = getuid())) == NULL)
    perror("getpwuid() error");
  
  groupid = (int)p->pw_gid;  

  return groupid;
  /*
  string line;
  string userFile = "userfile.txt";
  ifstream iFile(userFile.c_str());
  vector<string> grps;
  int checker = 0;
  while(getline(iFile, line) && checker==0)
    {
      vector<string> temp = split(line, ' ');
      if(temp.front() == username){
	int len = temp.size();
	for (int i = 1; i < len; i++){
	  grps.push_back(temp.at(i));
	}
      }
    }
  return grps;
  */
}

//gets a list of valid system users from the userfile.txt
vector<string> validusers(string filename){
  string line;
  int count = 0;
  vector<string> valid;
  ifstream iFile(filename.c_str());

  while(getline(iFile, line))
    {
      valid.push_back(split(line, ' ').front());
      count++;
    }
  return valid;
}

//checks for whether a given vector<string> contains a string
bool contains(vector<string> collection, string loner){
  int len = collection.size();
  int checker = 0;
  for (int i = 0; i < len; i++){
    if(collection.at(i) == loner){
      checker = 1;
    }
  }
  if(checker == 0){
    return false; 
  }else{
    return true;
  }
}

//checks specific user agains the list of valid user obtained from 'validusers' function
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

//splits a string with a delimeter
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


/*
  for each of the following functions 'canReadFile', 'canWriteFile', 'canReadAcl', 'canWriteAcl', 
  read the ACL file associated with the specific a file line by line.
  Determine what permissions a user has base on username and group affiliations.
  ACL file is in the following form:
     username.groupname permissions
*/ 
//general implementation of a function that checks a user's permissions agains an acl file. Backbone of all the other permission checking functions 
bool canDoSomething(string filename, string username, const char perm){
  int grps = userGroups();
  ifstream iFile(filename.c_str());
  string line;
  int checker = 0;
  int hasPerm = 0;
  while(getline(iFile, line) && checker==0 && hasPerm==0)
    {
      //int hasPerm = 0; 
      //breaks each line into two strings ... one in the form 'u.g' and the other in the form 'rwxpv' representing the user/group permissions 
      vector<string> both = split(line,' ');
      if(both.size()!=2 || split(both.at(0),'.').size()!=2){
	cout << "Acl file " << filename << " is not in the right format. \nEach line should be of the form 'username.group permissions'!\nModify this and try again.\n";
	exit(1);
      }
      string usr = split(both.at(0),'.').at(0);
      string grp = split(both.at(0),'.').at(1);
      //username and groups matching
      if(usr==username && grp=="*"){
	hasPerm = 1;
      }else if(grps==atoi(grp.c_str()) && usr=="*"){ 
	hasPerm= 1;
      }
      else if(usr==username && grps==atoi(grp.c_str()) ){
	hasPerm= 1;
      }
      else if(usr=="*" && grp=="*"){
	hasPerm = 1;
      }
      else{
	//this is everything else, no match .. keep looking  
      }

      //if we find permissions for specified user in acl
      if(hasPerm == 1){
	string permissions = both.at(1);
	int length = permissions.size();
	for(int i = 0; i < length; i++){
	  if (permissions.at(i) == perm)
	    checker = 1;
	}
      }
    }//end of while
  
  if(checker == 1){
    return true;
  }else{
    return false;
  }
}

//checks if user or group user belongs to has the 'r' permission   
bool canReadFile(string filename, string username){
  return canDoSomething(filename, username, 'r');
}

//checks if user or group user belongs to has the 'w' permission 
bool canWriteFile(string filename, string username){
  return canDoSomething(filename, username, 'w');
}

//checks if user or group user velongs to has 'x' permissions
bool canExecuteFile(string filename, string username){
  return canDoSomething(filename, username, 'x');
}

//checks if user or group user belongs to has the 'v' permission 
bool canReadAcl(string filename, string username){
  return canDoSomething(filename, username, 'v');
}

//checks if user or group user belongs to has the 'p' permission 
bool canWriteAcl(string filename, string username){
  return canDoSomething(filename, username, 'p');
}
