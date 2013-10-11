#include "functions.cpp"

int main(int argc, char *argv[])
{
  //checks to make sure that there is exac
  if (argc != 1){
    cout << "Incorrect number of args to objlist\n";
  }
  else{
    //obtains the username and group IDs from the VM via getpwuid                                                                                                                   
    struct passwd *p;
    uid_t  uid= getuid();
    
    if ((p = getpwuid(uid)) == NULL)
      perror("getpwuid() error");
    //get username and groupid from getpwuid
    string usr = p->pw_name;
    int groupid = (int)p->pw_gid;
    DIR *dir;
    struct dirent *ent;
    dir = opendir (".");
    if (dir != NULL) {
      // print all the files belonging to usr
      while ((ent = readdir (dir)) != NULL) {
	vector<string> fileparts= split(ent->d_name, '_');
	if(fileparts.at(0)==usr){
	  cout << ent->d_name << "\n";
	}
      }
      closedir (dir);
    } else {
      //if error
      perror ("");
      return EXIT_FAILURE;
    }
    return 0;
  }
}
  
