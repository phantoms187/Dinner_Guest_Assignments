#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int table_likeness_score(int chairs, int **people, int assignment[])
{
  int sum = 0;
  int half_chairs = chairs/2;

  for(int i = 0; i < 2; ++i)
  {
    for(int j = 0; j < half_chairs-1; ++j)
    {
      sum += people[assignment[(half_chairs*i)+j]][assignment[(half_chairs*i)+j+1]];
      cout << "\nsum: " << sum << "\nAssignment: " << assignment[(half_chairs*i)+j];

      sum += people[assignment[(half_chairs*(i+1)-(j+1))]][assignment[(half_chairs*(i+1)+(j-2))]];
      cout << "\nsum after: " << sum << "\nAssignment: " << assignment[(half_chairs*(i+1)-(j+1))];
    }
  }
  
  return sum;
}



int main(int argc, char** argv)
{
  int chairs = 0;
  int ** people;

  ifstream myfile (argv[1]);

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
  
  int assignment[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; 
  
  int sum = table_likeness_score(chairs, people, assignment);

  cout << "\nSum is: " << sum << "\n";

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
