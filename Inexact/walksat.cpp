#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <time.h>

#define TRYES 500
#define STEPS 10000
#define PENALTY 1000

using namespace std;

bool walksat(vector<vector<int>> &matrix, vector<int> &variables, int num, int maxTries, int maxSteps);
bool satisfies(vector<vector<int>> &matrix, vector<int> &variables, int max);
bool init_inc(vector<vector<int>> &matrix, int num);
int heuristic(vector<int> constraint, int);

int row = 0, column = 0;
vector<bool> hash_constraint;
vector<vector<int>> inc;
pair<int, int> ans;
vector<int> ans_true;

int main(int argc, char* argv[])
{
	srand(time(NULL));

	if (argc < 3)
	{
		cout << "\t|--ERROR enter filename\n";
		return 0;
	}

	fstream file;
	string line;
	int n = 0;
	vector<vector<int>> matrix;
	file.open(argv[1], ios::in);
	for( ; ; )
	{
		std::getline(file, line);
		if(!file)   // test eof
			break;
		matrix.push_back(vector<int>());
		istringstream fline(line);
		column = 0;
		for( ; ; ) {
			int val;
			fline >> val;
			if(!fline)
				break;
			if(val != 0)
				matrix[row].push_back(val - 1);
			column++;
		}
		row++;
		if(n == 0)
			n = column;
	}

	// test for being true
	for(int i = 0; i < matrix.size(); i++) {
		for(int j = 0; j < matrix[i].size(); j++) {
			cout << matrix[i][j] << " ";
	   }
	   cout << endl;
	}

	int num = 0;
	for(int i = 0; i < matrix.size(); i++) {
		for(int j = 0; j < matrix[i].size(); j++) {
			if(matrix[i][j] > num)
				num = matrix[i][j];
		}
	}
	num++;
	vector<int> variables(num);

	init_inc(matrix, num);
	hash_constraint.resize(matrix.size());
	ans.first = matrix.size();
	ans.second = num;

	walksat(matrix, variables, num, TRYES, STEPS);

	freopen(argv[2], "w", stdout);

	printf("\nNO of unsatified: %d\nNO of true vars: %d\n", ans.first, ans.second);

	for (int i = 0; i < ans_true.size(); i++)
	    cout << ans_true[i] << " ";
	cout << endl;

	//cout << "least_one: " << least_one << endl;

	return 0;
}

bool walksat(vector<vector<int>> &matrix, vector<int> &variables, int num, int maxTries, int maxSteps)
{
	for(int t = 0; t < maxTries; t++) {
		printf("\t|--TRY %d\n", t);
		// random initialize
		for(int i = 0; i < num; i++) {
			//variables[i] = rand() % 2;
			if(t != maxTries - 1)
				variables[i] = 0;
			else
				variables[i] = 1;
		}
		variables[t % num] = 1;
		variables[(t / num) % num] = 1;
		for(int step = 0; step < maxSteps; step++) {
			if(satisfies(matrix, variables, num))
				return true;

			// select a random unsatified clause
			int unsatified = 0;
			for(int i = 0; i < matrix.size(); i++) {
				int cnt = 0;
				for(int j = 0; j < matrix[i].size(); j++) {
					int index = matrix[i][j];
					cnt += variables[index];
				}
				if(cnt % 2 == 1)
				{
					unsatified++;
					hash_constraint[i] = 0;
				}
				else
					hash_constraint[i] = 1;
			}

			int total = 0;
			for (int i = 0; i < num; i++)
				if (variables[i])
					total++;

			if ((total && ans.first * PENALTY + ans.second > unsatified * PENALTY + total) || 
				(total && ans.first * PENALTY + ans.second == unsatified * PENALTY + total && ans.first > unsatified))
			{
				ans.first = unsatified;
				ans.second = total;

				ans_true.clear();
				for (int i = 0; i < num; i++)
					if (variables[i])
						ans_true.push_back(i);
			}

			//cout << endl << "\t|--unsatified: " << unsatified << " |--total: " << total << endl;

			int rand_clause = 0, selected = 0;
			if (!unsatified)
			{
				rand_clause = rand() % matrix.size();
				selected = rand_clause;
			}

			else
			{
				rand_clause = rand() % unsatified;

				int j = 0;
				for(int i = 0; j < unsatified; i++)
				{
					if (!hash_constraint[i])
					{
						//cout << i << " ";
						if (j == rand_clause)
							selected = i;
						j++;
					}
				}
			}

			int var_index = heuristic(matrix[selected], total);
			variables[var_index] ^= 1;
		}
	printf("\nNO of unsatified: %d\nNO of true vars: %d\n", ans.first, ans.second);

	}
}

bool satisfies(vector<vector<int>> &matrix, vector<int> &variables, int max)
{
	bool satisfies = true;
	int cnt;
	for(int i = 0; i < matrix.size(); i++) {
		cnt = 0;
		for(int j = 0; j < matrix[i].size(); j++) {
			int index = matrix[i][j];
			cnt += variables[index];
		}
		if(cnt % 2 == 1)
			return false;
	}

	// chaeck that all variables are not equal to zero
	int sum = 0;
	for(int i = 0; i < max; i++) {
		sum += variables[i];
	}
	if(sum == 0)
		return false;

	return true;
}

bool init_inc(vector<vector<int>> &matrix, int num)
{
	inc.resize(num);
	for(int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			inc[j].push_back(i);
		}
	}
}

int heuristic(vector<int> constraint, int noone)
{
	if (noone == 1)
	{
		int r = rand() % (constraint.size());
		cout << "\t|-- r: " << r << " |--var: " << constraint[r] << endl;
		return constraint[r];
	}
	
	int i = 0, j = 0, m = 0, total = 0;
	bool select = true;
	vector<int> nouns(constraint.size());   //count number of satisfied

	for (i = 0; i < constraint.size(); i++)
	{
		nouns[i] = 0;
		select = true;
		for (j = 0; j < inc[constraint[i]].size(); j++)
		{
			if (hash_constraint[inc[constraint[i]][j]])
			{
				select = false;
				nouns[i]++;
				if (nouns[i] > m)
					m = nouns[i];
			}
		}

		if (select)
			return constraint[i];
	}

	for (i = 0; i < nouns.size(); i++)
	{
		nouns[i] = m - nouns[i] + 1;
		nouns[i] *= 3;
		total += nouns[i];
	}

	//choose random, high probability on less satisfied
	j = rand() % total;

	for (i = 0; i < nouns.size(); i++)
	{
		if (j < nouns[i])
			return constraint[i];
		else
			j -= nouns[i];
	}

	cout << "\t|--ERRROR no variable returned" << endl;
	return constraint[0];
}
