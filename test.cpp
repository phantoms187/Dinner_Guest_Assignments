#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <set>
#include <iterator>

using namespace std;

struct Pair{
  int first, second, score;
};

bool host_and_guest(int half_chairs, int chair1, int chair2)
{
  return (chair1 < half_chairs && chair2 >= half_chairs || chair1 >= half_chairs && chair2 < half_chairs);
}

int mutual_likeness(int** people, int first, int second)
{
  return people[first][second] + people[second][first];
}

bool comparePairs(Pair first, Pair second)
{
  return (first.score < second.score);
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
  }
  if(host_and_guest(half_chairs, assignment[current_chair], assignment[current_chair+half_chairs]))
     sum += 2;

  return sum;
}

void display(vector<int> a)
{
    for (auto i : a) {
        cout << i << "  ";
    }
    cout << endl;
}
//Found on geeksforgeeks.org
int findPermutations(vector<int> most_liked, vector<int> most_unliked, int assignment[], int chairs, int **people)
{
    int total = 0;
    // Sort the given array
    sort(most_liked.begin(), most_liked.end());

    // Find all possible permutations
    cout << "Possible permutations are:\n";
    do {
        for(int i = 0; i < chairs; ++i)
        {
          assignment[i] = most_liked[i];
          //assignment[2*i+1] = most_unliked[i];
        }
        if(table_likeness_score(chairs, people, assignment) > total)
          total = table_likeness_score(chairs, people, assignment);
      //  cout << total << ": ";
      //  display(most_liked);
    } while (next_permutation(most_liked.begin(), most_liked.end()));

    return total;
}





int main(int argc, char** argv)
{
  int chairs, sum = 0;
  int most_unliked_count = 0;

  int ** people;
  vector<Pair> like_Pairs;
  vector<int> most_unliked;
  vector<int> most_liked;

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

  int assignment[chairs] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,};

//Build set with all people coming to dinner
  for(int i = 0; i < chairs; ++i)
  {
    assignment_pool.insert(i);
  }

//Add each possible pair with their net likeness score to vector like_Pairs
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

//Sort pairs by likeness net score from worst to best
  sort(like_Pairs.begin(), like_Pairs.end(), comparePairs);

//Determine most unliked people from worst pairs and store in vector
  for(int i = 0; i < chairs; ++i)
  {
      if(assignment_pool.find(like_Pairs[i].first) != assignment_pool.end())
      {
        most_unliked.push_back(like_Pairs[i].first);
        ++most_unliked_count;
        assignment_pool.erase(like_Pairs[i].first);
        if(most_unliked_count >= chairs/2)
          break;
      }
      if(assignment_pool.find(like_Pairs[i].second) != assignment_pool.end())
      {
        most_unliked.push_back(like_Pairs[i].second);
        ++most_unliked_count;
        assignment_pool.erase(like_Pairs[i].second);
        if(most_unliked_count >= chairs/2)
          break;
      }

  }


  //for(auto i : assignment_pool)

  for(int i = 0; i < chairs; ++i)
  {
    most_liked.push_back(i);
  }

  cout << endl << "most_unliked: " << endl;
  for(auto i : most_unliked)
  {
    cout << i << ", ";
  }

  cout << endl << "most_liked: " << endl;
  for(auto i : most_liked)
  {
    cout << i << ", ";
  }

  // for(int i = 0; i < chairs/2; ++i)
  // {
  //   assignment[2*i] = most_liked[i];
  //   assignment[2*i+1] = most_unliked[i];
  // }
  // cout << endl;
  // cout << "chairs: " << endl;
  // for(int i = 0; i < chairs; ++i)
  // {
  //   cout << assignment[i] << ", ";
  // }
  // cout << endl;

  //int size = most_liked.size();//end() - most_liked;
  sum = findPermutations(most_liked, most_unliked, assignment, chairs, people);


  //sum = table_likeness_score(chairs, people, assignment);

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
