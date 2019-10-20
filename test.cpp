#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
  int total_people = 0;
  
  cout << argv[1] <<"\n";

  string line;
  ifstream myfile (argv[1]);
  if(myfile.is_open())
  {
    myfile >> total_people;
    int people[total_people][total_people];

    for(int i = 0; i < total_people; ++i)
    {
      for(int j = 0; j < total_people; ++j)
      {
        myfile >> people[i][j];
        cout << people[i][j] << " ";
         if(j == total_people-1)
           cout << "\n";
      }

    }
    cout << "Person 1 likes person 2: " << people[0][1] << "\n";
    cout << "Person 3 likes person 7: " << people[2][6] << "\n";


    myfile.close();
  }

  else cout << "Unable to open file";

  cout << "\nTotal people: " << total_people << "\n";



  return 0;

}
