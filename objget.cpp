#include "functions.cpp"

int main(int argc, char *argv[])
{
  //checks to see whether program has the correct number of arguements
  if (argc != 2){
    cout << "Incorrect number of args to objget\n"; 
  }
  else{
    //obtains the username and group IDs from the VM via getpwuid                                                                                                                   
    struct passwd *p;
    uid_t  uid;
    
    if ((p = getpwuid(uid = getuid())) == NULL)
      perror("getpwuid() error");
    
    string username = p->pw_name;
    int groupid = (int)p->pw_gid;
    string objname = argv[1];
    string tempfilename;
    string aclName;
    //check for whether we're trying to open an object that belongs to another user
    vector<string> temp = split(objname, '+'); 
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
    input_data.open(filename);
    //check whether we have permission to read the file
    if (!canReadFile(aclName, username)) {
      cout << "Nah! Can't read " << filename << "\n";
    }//no file
    else if ( ! input_data ) {
      cout << "Error: Can't open the file named " << filename << ".\n";
      exit(1);
    }
    else{//do this if user has permission to read file
      int pos = 0;
      string str;
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
