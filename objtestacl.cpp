#include "functions.cpp"

int main(int argc, char *argv[])
{
  //checks to see if correct num of args
  if (argc != 4){
    cout << "Incorrect number of args to objtestacl\n"; 
  }
  else{
    struct passwd *p;
    uid_t  uid=getuid();
    //get using information via getpwuid()
    if ((p = getpwuid(uid)) == NULL)
      perror("getpwuid() error");

    string username = p->pw_name;
    int groupid = (int)p->pw_gid;
    string objname = argv[3];
    string tempfilename;
    string aclName;
    vector<string> temp = split(objname, '+');
    //checks if user is owner of file
    if(temp.size() == 2){
      string t_username =  temp.at(0);
      objname =  temp.at(1);
      tempfilename = t_username + "_" + objname + ".txt";
      aclName = t_username + "_" + objname + "_acl.txt";
    }else{
      tempfilename = username + "_" + objname + ".txt";
      aclName =  username + "_" + objname + "_acl.txt";
    }
    cout << aclName << "\n";
    //checks against functions in functions.cpp for the permission that the user has
    if (canReadFile(aclName, username) || canWriteFile(aclName, username) || canExecuteFile(aclName, username) ||  canReadAcl(aclName, username) || canWriteAcl(aclName, username)){
      cout << "allowed with the following pemissions:\n";
      if(canReadFile(aclName, username)){
	cout << "r";
      }
      if(canWriteFile(aclName, username)){
	cout << "w";
        }
      if(canExecuteFile(aclName, username)){
	cout << "x";
      }
      if(canReadAcl(aclName, username)){
	cout << "v";
      }
      if(canWriteAcl(aclName, username)){
	cout << "p";
      }
      cout <<  "\n";
    }else{
      //if no permissions output denied
      cout << "denied\n";
    }
  }
  return 0;
}
