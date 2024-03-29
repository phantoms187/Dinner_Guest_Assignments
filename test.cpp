#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <set>
#include <iterator>


struct Pair{
  int first, second, score;
};

int mutual_likeness(int** people, int first, int second)
{
  return people[first][second] + people[second][first];
}

bool comparePairs(Pair first, Pair second)
{
  return (first.score < second.score);
}

using namespace std;

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
    }
  }
//Add scores for people across from each other
  for(int i = 0; i < half_chairs; ++i)
  {
    current_chair = i;

    sum += people[assignment[current_chair]][assignment[current_chair+half_chairs]];
    sum += people[assignment[current_chair+half_chairs]][assignment[current_chair]];
  }

  return sum;
}

int main(int argc, char** argv)
{
  int chairs = 0;
  int ** people;
  vector<Pair> like_Pairs;
  set<int> assignment_pool;


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

  for(int i = 0; i < chairs; ++i)
  {
    assignment_pool.insert(i);
  }

  cout << "Assignments: \n";
  set <int> :: iterator itr;
  cout << "\nThe set gquiz1 is : ";
   for (itr = assignment_pool.begin(); itr != assignment_pool.end(); ++itr)
   {
       cout << '\t' << *itr;
   }
   cout << endl;

  for(int i = 0; i < chairs-1; ++i)
  {
    for(int j = i+1; j < chairs; ++j)
    {
      Pair current;
      current.first = i;
      current.second = j;
      current.score = mutual_likeness(people, i, j);
      like_Pairs.push_back(current);
    }
  }

  cout << "Pairs sorted by: \n";
   for (auto x : like_Pairs)
       cout << "[" << x.first << ", " << x.second << ", " << x.score << "] " << endl;

cout << "\n-------------------------------------------------------------------------\n";
  sort(like_Pairs.begin(), like_Pairs.end(), comparePairs);

  cout << "Pairs sorted by: \n";
   for (auto x : like_Pairs)
       cout << "[" << x.first << ", " << x.second << ", " << x.score << "] " << endl;


    int assignment[chairs] = {0,5,1,6,2,7,3,8,4,9};

  // for(int i = 0; i < chairs; ++i)
  // {
  //    assignment[i] = i;
  // }


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
