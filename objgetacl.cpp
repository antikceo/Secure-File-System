#include "functions.cpp"

int main(int argc, char *argv[])
{
  //checks to make sure that there are exactly 2 args, 1 for the 'objput' command and the other for the objname                                                                     
  if (argc != 2){
    cout << "Incorrect number of args to objput\n";
  }
  else{
    //obtains the username and group IDs from the VM via getpwuid                                                                                                                   
    struct passwd *p;
    uid_t  uid= getuid();

    if ((p = getpwuid(uid)) == NULL)
      perror("getpwuid() error");

    string username = p->pw_name;
    int groupid = (int)p->pw_gid;
    string objname = argv[1];
    string tempfilename;
    string aclName;
    vector<string> temp = split(objname, '+');
    //if user is trying to access another user's acl's
    if(temp.size() == 2){
      string t_username =  temp.at(0);
      objname =  temp.at(1);
      tempfilename = t_username + "_" + objname + ".txt";
      aclName = t_username + "_" + objname + "_acl.txt";
    }else{
      tempfilename = username + "_" + objname + ".txt";
      aclName =  username + "_" + objname + "_acl.txt";;
    }
    
    const char * filename = tempfilename.c_str();
    ifstream input_data;
    input_data.open(aclName.c_str());
    //checks for whether user can read the ACL requested
    if(!canReadAcl(aclName, username)){
      cout << username << " does not have permission to view the ACL " << aclName << "\n";
    } 
    else if ( ! input_data ) {
      cout << "Error: Can't open the file named " << filename << ".\n";
      exit(1);
    }
    else{
      int pos = 0;
      string str;
      //prints out file content to console
      cout << "File content below:\n";
      getline(input_data,str);
      while (input_data ) {
	cout << str << "\n";
	getline(input_data,str);
      }
    }
  }
  return 0;
}
