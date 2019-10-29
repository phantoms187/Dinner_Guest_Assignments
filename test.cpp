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

//Struct for each pair to determine worst pairs to sit together
struct Pair{
  int first, second, score;
};

//Determine if a pair is a host and guest combination
bool host_and_guest(int half_chairs, int chair1, int chair2)
{
  return (chair1 < half_chairs && chair2 >= half_chairs || chair1 >= half_chairs && chair2 < half_chairs);
}

//Determine the mutual likeness between two filled seats
int mutual_likeness(int** people, int first, int second, int chairs)
{
  int total = 0;
  if(host_and_guest(chairs/2,first, second))
    ++total;
  total += (people[first][second] + people[second][first]);

  return total;
}

//Comparison used for sorting
bool comparePairs(Pair first, Pair second)
{
  return (first.score < second.score);
}

//Score calculated once table is assigned
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

//Assign table by first placing the worst pairs then randomly assigning everyone else
int find_best_table(vector<int> most_liked, vector<int> most_unliked, int assignment[], int temp[], int chairs, int **people)
{
    int sum, next = 0;
    int max = -1000;
   
//From stackoverflow to generate time based seed
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine liked_seed(seed);
    default_random_engine unlike_seed(seed);
    
  //Each iteration through the loop the most_unliked guests get shuffled  
    for(int h = 0; h < 100; ++h)
    {  
      //Each iteration through the loop the most_liked guests get shuffled       
        for(int k = 0; k < 10000; ++k)
        {
          shuffle(most_liked.begin(), most_liked.end(), liked_seed);
          
          //Assign to non-adjacent seats the guests in most_unliked
          for(int i = 0; i < most_unliked.size(); ++i)
          {
            assignment[2*i] = most_unliked[i];
          }
          
          //Assign the remaining guests to open seats
          for(int i = 0; i < chairs; ++i)
          {
              if(assignment[i] == -1)
              {
                  assignment[i] = most_liked[next];
                  ++next;
              }
          }
          
          //Determine current table score 
          sum = table_likeness_score(chairs, people, assignment);
          
          //If current table score is higher than current max score, save and store table assignment
          if(sum > max)
          {
              max = sum;
              for(int j = 0; j < chairs; ++j)
              { 
                  temp[j] = assignment[j];
              }    
          } 
          //Reset assignments to -1 to show empty assignments
          for(int i = 0; i < chairs; ++i)
          {
              assignment[i] = -1;
          }
          //Reset next counter for most_like assignments
          next = 0;
            
        }
        //Shuffle the most_unliked guests
        shuffle(most_unliked.begin(), most_unliked.end(), unlike_seed);
    }
    return max;
}

int main(int argc, char** argv)
{
  
  int chairs = 0;
  //count to stop addint guests to unliked list
  int most_unliked_count = 0;
  int max = -10000;
  //Matrix of the read in values for the table assignments
  int ** people;
  //Vectors for the two lists of unliked/liked guests and the list of 
  //pairs to decide which guests go where
  vector<Pair> like_Pairs;
  vector<int> most_unliked;
  vector<int> most_liked;

  //Set to check if a guest has been assigned yet
  set<int> assignment_pool;

  //Get file from command line argument
  ifstream myfile (argv[1]);

  //Open file and save to peopl matrix
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

//Set original assignments to -1 for emptiness checking  
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

//Build most unliked list
  for(int i = 0; i < chairs; ++i)
  {
      //Check each guest starting with worst pair and add to most_unliked
      //Only adding a quarter of the guests to not limit random search too much
      if(assignment_pool.find(like_Pairs[i].first) != assignment_pool.end())
      {
        most_unliked.push_back(like_Pairs[i].first);
        ++most_unliked_count;
        assignment_pool.erase(like_Pairs[i].first);
        if(most_unliked_count >= chairs/4)
          break;
      }
      if(assignment_pool.find(like_Pairs[i].second) != assignment_pool.end())
      {
        most_unliked.push_back(like_Pairs[i].second);
        ++most_unliked_count;
        assignment_pool.erase(like_Pairs[i].second);
        if(most_unliked_count >= chairs/4)
          break;
      }
  }

//Put remaining guests not in most_unliked to most_liked group 
  for(auto i : assignment_pool)
  {
    most_liked.push_back(i);
  }

//Use temp array to store best table assignment  
  int temp[chairs];
  
//Call function to generate assignments and return best score found
  max = find_best_table(most_liked, most_unliked, assignment, temp, chairs, people);

  for(int i = 0; i < chairs; ++i)
  {
    cout << "Person " << temp[i]+1 << " is in seat: " << i + 1 << endl;
  }
  
  cout << "\nScore is: " << max << endl;


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
