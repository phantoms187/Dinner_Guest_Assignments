#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <set>
#include <iterator>
#include <algorithm>
#include <random>

using namespace std;

struct Pair{
  int first, second, score;
};

bool host_and_guest(int half_chairs, int chair1, int chair2)
{
  return (chair1 < half_chairs && chair2 >= half_chairs || chair1 >= half_chairs && chair2 < half_chairs);
}

int mutual_likeness(int** people, int first, int second, int chairs)
{
  int total = 0;
  if(host_and_guest(chairs/2,first, second))
    ++total;
  total += (people[first][second] + people[second][first]);

  return total;
}

bool comparePairs(Pair first, Pair second)
{
  return (first.score > second.score);
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
int findPermutations(vector<int> most_liked, vector<int> most_unliked, int assignment[], int temp[], int chairs, int **people)
{
    int sum = 0;
    int max = -1000;
    // Sort the given array
    //sort(most_liked.begin(), most_liked.end());
//From stackoverflow to generate time based seed
      unsigned seed = chrono::system_clock::now().time_since_epoch().count();
      default_random_engine liked_seed(seed);
      ++seed;
      default_random_engine unlike_seed(seed);
      for(int i = 0; i < 10000000; ++i)
      {
        //srand(unsigned(time(0)));

        shuffle(most_liked.begin(), most_liked.end(), liked_seed);
        //shuffle(most_unliked.begin(), most_unliked.end(), unlike_seed);
        for(int i = 0; i < chairs/2; ++i)
        {
          assignment[2*i] = most_liked[i];
          assignment[2*i+1] = most_unliked[i];
        }

        sum = table_likeness_score(chairs, people, assignment);

        if(sum > max)
          max = sum;

      }




    //
    //       for(int i = 0; i < chairs/2; ++i)
    //     {
    //       assignment[2*i] = most_liked[i];
    //       assignment[2*i+1] = most_unliked[i];
    //     }
    //     //total = table_likeness_score(chairs, people, assignment);
    //     if(total > max)
    //     {
    //         max = total;
    //         for(int j = 0; j < chairs; ++j)
    //         {
    //           temp[j] = assignment[j];
    //         }
    //     }
    //     //cout << total << ": ";
    //     //display(most_liked);
    // }

    return max;
}

// void setBestAssignment(int assignment[]; int chairs, vector<Pair> like_pairs)
// {
//   for(int i = 0; )
// }

int main(int argc, char** argv)
{
  int chairs, sum = 0;
  int most_unliked_count = 0;
  int max = -10000;

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

  int assignment[chairs];
  for(int i = 0; i < chairs; ++i)
  {
    assignment[i] = -1;
  }

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
      current.score = mutual_likeness(people, i, j, chairs);
      like_Pairs.push_back(current);
    }
  }

//Sort pairs by likeness net score from best to worst
  sort(like_Pairs.begin(), like_Pairs.end(), comparePairs);

  // cout << "Pairs sorted by: \n";
  //  for (auto x : like_Pairs)
  //      cout << "[" << x.first << ", " << x.second << ", " << x.score << "] " << endl;


cout << endl << "most_unliked: " << endl;
for(int i = 0; i < chairs; ++i)
{
  cout << assignment[i] << ", ";
}

assignment[0] = (like_Pairs[0].first);
assignment_pool.erase(like_Pairs[0].first);
assignment[(chairs/2)] = (like_Pairs[0].second);
assignment_pool.erase(like_Pairs[0].second);

int j = like_Pairs.size();

    while(!(assignment_pool.find(like_Pairs[j-1].first) != assignment_pool.end() &&
            assignment_pool.find(like_Pairs[j-1].second) != assignment_pool.end()))
    {
      --j;
    }
    assignment[2] = (like_Pairs[j-1].first);
    assignment_pool.erase(like_Pairs[j-1].first);
    assignment[3+(chairs/2)] = (like_Pairs[j-1].second);
    assignment_pool.erase(like_Pairs[j-1].second);

    cout << endl << "most_unliked: " << endl;
    for(int i = 0; i < chairs; ++i)
    {
      cout << assignment[i] << ", ";
    }

//
for(auto i : assignment_pool)
// for(int i = 0; i < chairs; ++i)
{
  most_liked.push_back(i);
}


unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine liked_seed(seed);
int temp[chairs];
for(int i = 0; i < chairs; ++i)
{
  temp[i] = assignment[i];
}

for(int k = 0; k < 10000; ++k)
{
  j=0;
  shuffle(most_liked.begin(), most_liked.end(), liked_seed);

  for(int i = 1; i < chairs; ++i)
  {
    if(temp[i] == -1)
    {
      assignment[i] = most_liked[j];
    //  assignment_pool.erase(most_liked[j]);
      ++j;
    }
  }
  
  sum = table_likeness_score(chairs, people, assignment);

  if(sum > max)
    max = sum;
}

// cout << endl << "most_unliked: " << endl;
// for(int i = 0; i < chairs; ++i)
// {
//   cout << assignment[i] << ", ";
// }


  //
  // cout << endl << "most_unliked: " << endl;
  // for(int i = 0; i < chairs; ++i)
  // {
  //   cout << assignment[i] << ", ";
  // }
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
  cout << endl;
  // cout << "chairs: " << endl;
  // for(int i = 0; i < chairs; ++i)
  // {
  //   cout << assignment[i] << ", ";
  // }
  // cout << endl;

  //int size = most_liked.size();//end() - most_liked;
  //int temp[chairs];
  //sum = findPermutations(most_liked, most_unliked, assignment, temp, chairs, people);
  //int max = -10000;
//From stackoverflow to generate time based seed
  // unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  // default_random_engine e(seed);
  // for(int i = 0; i < 10000000; ++i)
  // {
  //   //srand(unsigned(time(0)));
  //
  //   shuffle(most_liked.begin(), most_liked.end(), e);
  //
  //   sum = table_likeness_score(chairs, people, most_liked);
  //
  //   if(sum > max)
  //     max = sum;
  //
  // }

  cout << "\nScore is: " << max << "\n";

  // for(int i = 0; i < chairs; ++i)
  // {
  //   cout << "Person " << temp[i]+1 << "  is sitting in seat: " << i + 1 << endl;
  // }

  // sum = table_likeness_score(chairs, people, temp);
  //


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
