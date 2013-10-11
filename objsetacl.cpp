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
    uid_t  uid=getuid();
    
    if ((p = getpwuid(uid)) == NULL)
      perror("getpwuid() error");

    string username = p->pw_name;
    int groupid = (int)p->pw_gid;
    string objname = argv[1];
    string aclName;
    string tempfilename;
    vector<string> temp = split(objname, '+');
    //checks to see if the user is trying to read another user's acl
    if(temp.size() == 2){
      string t_username =  temp.at(0);
      objname =  temp.at(1);
      tempfilename = t_username + "_" + objname + ".txt";
      aclName = t_username + "_" + objname + "_acl.txt";
    }else{
      tempfilename = username + "_" + objname + ".txt";
      aclName =  username + "_" + objname + "_acl.txt";;
    }
    
    const char * filename = aclName.c_str();
    ofstream out;
    string content;
    //checks if user has write permissions to the ACL requested
    if(!canWriteAcl(aclName, username)){
      cout << username << " does not have permission to write to the ACL " << aclName << "\n";
    }else{
      cout << "If you would like to enter ACL input from console enter 'c' else if you want to copy from an existing file enter 'f'\n";
      string choice;
      getline(cin, choice);
      if(choice == "c"){
	out.open(filename);
	//give user options on how to give input to ACL
	cout << "Please enter the text you would like to write to the ACL file, " << filename << " in the form user.group permissions:\n";
	getline(cin, content);
	out<<content;
	out.close();
	cout << "Done writing to ACL file " << filename << ".\nGoodbye!\n";
      }else if(choice == "f"){
	cout << "enter the name of the .txt ACL file you would like to write from:\n";
	string inputFile;
	getline(cin, inputFile);
	//checks if user can read file
	if(!canReadiFile(inputFile)){
	  cout << "You either do not have permission to read " << inputFile << " or it does not exist.";
	}else{
	  if(!canReadAcl(inputFile, username)){
	    cout << "You don't have permission to read this ACL file or it does not exist in a valid namespace, therefore you cannot copy it!";
	    exit(1);
	  }
	  copyFile(inputFile, aclName);
	}
      }else{
	//if user enters invalid option
	cout << choice << " is not a valid option!";
      }
    }
  }
  return 0;
}
