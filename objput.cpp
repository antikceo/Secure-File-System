#include "functions.cpp"

int main(int argc, char *argv[])
{
  //checks to make sure that there are exactly 2 args, 1 for the 'objput' command and the other for the objname 
  if (argc != 2){
    cout << "Incorrect number of args to objput\n"; 
  }
  else{
    //int muid = getuid();
    //printf( "%d\n", getuid() );
    //obtains the username and group IDs from the VM via getpwuid
    struct passwd *p;
    uid_t uid;
    string username;
    int groupid;
    string objname;

    if ((p = getpwuid(uid = getuid())) == NULL)
      perror("getpwuid() error");

    username = p->pw_name;
    groupid = (int)p->pw_gid;
    objname = argv[1];
    string aclName;
    string tempfilename;
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
    ofstream out;
    ifstream input_data;
    input_data.open(filename);
    //checks users write permissions
    if(!canWriteFile(aclName, username) && input_data){
      cout << username << " does not have permission to write to \n" << filename;
    }else{
      cout << "If you would like to enter input from console enter 'c' else if you want to copy from an existing file enter 'f'\n";
      string choice;
      getline(cin, choice);
      if(choice == "c"){
	string content;
	out.open(filename);
	cout << "Please enter the text you would like to write to " << filename << " below:\n";
	getline(cin, content);
	out<<content;
	out.close();
	
	//create initial ACL for file                                                                                                                                           
	out.open(aclName.c_str());
	out << username << ".* rwxpv\n*." << groupid << " rwxpv\n*.* x";
	out.close();
	chmod(filename, S_IRWXU | S_IXOTH);
	chmod(aclName.c_str(), S_IRWXU | S_IRWXO);
	cout << "Done writing to " << filename << ".\nAlso create initial ACL with file name " << aclName << ".\nGoodbye!\n";
      }else if(choice == "f"){
	cout << "enter the name of the .txt file you would like to write from:\n";
	string inputFile;
	getline(cin, inputFile);
	//checks if user can read file
	if(!canReadiFile(inputFile)){
	  cout << "You either do not have permission to read " << inputFile << " or it does not exist.";
	}else{
	  string aclNamed = split(inputFile, '.').at(0) + "_acl.txt";
	  //if user does not have permission to read the file he/she is trying to copy
	  if(canReadiFile(aclNamed) && !canReadFile(aclNamed, username)){
	    cout << "You don't have permission to read this file, therefore you cannot copy it!";
	    exit(1);
	  }
	  copyFile(inputFile, tempfilename);
	}
      }else{
	cout << choice << " is not a valid option!"; //if user chooses an invalid option
      }
    }
  }
  return 0;
}
