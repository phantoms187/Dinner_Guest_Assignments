#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int table_likeness_score(int chairs, int **people, int assignment[])
{

  for(int i = 0; i < 2; ++i)
  {
    for(int j = 0; j < chairs; ++j)
    {

    }
  }
}



int main()
{
  int chairs = 0;
  int ** people;

  ifstream myfile ("text.txt");

  if(myfile.is_open())
  {
    myfile >> chairs;

    people = new int *[chairs];
    for(int i = 0; i < chairs; ++i)
    {
        people[i] = new int [chairs];
    }

    for(int i = 0; i < chairs; ++i)
    {
      for(int j = 0; j < chairs; ++j)
      {
        myfile >> people[i][j];
        /*cout << people[i][j] << " ";
         if(j == chairs-1)
           cout << "\n";
        */
      }

    }

    myfile.close();
  }

  else cout << "Unable to open file";

  cout << "\nPerson 3 likes Person 5 this much: " << people[2][4] << "\n";

  cout << "\nTotal people: " << chairs << "\n";

//Delete people dynamic matrix variable
if(people)
{
  for(int i = 0; i < chairs; ++i)
  {
      delete[] people[i];
  }
  delete[] people;
  people = 0;
}

  return 0;

}
