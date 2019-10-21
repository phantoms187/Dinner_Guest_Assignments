#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;


int host_and_guest(int half_chairs, int chair1, int chair2)
{
  if(chair1 < half_chairs && chair2 >= half_chairs || chair1 >= half_chairs && chair2 < half_chairs)
    return 1;
  else
    return 0;
}

int table_likeness_score(int chairs, int **people, int assignment[])
{
  int sum = 0;
  int half_chairs = chairs/2;
  int current_chair = -1;
  int last_chair = -1;

//Calculate scores for people sitting next to each other
  for(int i = 0; i < 2; ++i)
  {
    for(int j = 0; j < half_chairs-1; ++j)
    {
      current_chair = (half_chairs*i)+j;
      last_chair = (half_chairs*(i+1)-(j+1));

      sum += people[assignment[current_chair]][assignment[current_chair+1]];
      sum += people[assignment[last_chair]][assignment[last_chair-1]];

      if(host_and_guest(half_chairs, assignment[current_chair], assignment[current_chair + 1]))
         sum += 1;
    }
  }
//Add scores for people across from each other
  for(int i = 0; i < half_chairs; ++i)
  {
    current_chair = i;

    sum += people[assignment[current_chair]][assignment[current_chair+half_chairs]];
    sum += people[assignment[current_chair+half_chairs]][assignment[current_chair]];

    if(host_and_guest(half_chairs, assignment[current_chair], assignment[current_chair+half_chairs]))
      sum += 2;

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
      }
    }

    myfile.close();
  }
  else cout << "Unable to open file";



  int assignment[chairs];// = {0,5,1,6,2,7,3,8,4,9};

  for(int i = 0; i < chairs; ++i)
  {
     assignment[i] = i;
  }

  unsigned seed = 10;
  shuffle(assignment, assignment+chairs, default_random_engine(seed));
  cout << "\nChairs now: ";
  for(int i = 0; i < chairs; ++i)
  {
     cout << assignment[i] << ", ";
  }
  cout << "\n";



  int sum = table_likeness_score(chairs, people, assignment);

  cout << "\nTotal People: " << chairs << "\nSum is: " << sum << "\n";

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
