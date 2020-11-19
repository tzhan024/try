#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
using namespace std;
int main(int argc, char **argv)
{
    int i = 0;
    char infiles[5][20];
    //cout<<"Hello world"<<endl;
    //cout<<argc<<endl;
    if(argc > 2)
    {
        cout<<"Only one argument needed."<<endl;
        return 0;
    }
    else
    {
        ifstream file;
        file.open(argv[argc - 1]);
        
        if (file.is_open())
        {
            while (!file.eof())
            {
                file>>infiles[i];
                //cout<<infiles[i]<<endl;
                i++;
            }
        }
        file.close();
        //char *filename = argv[argc];
        int k = 0;
        set<int> index[100];
        map<string, set<int> > invertedIndex;
		string words[100];
		int num[100];
        for(int j = 0; j < i; j++)
        {
            ifstream in[i];
            in[j].open(infiles[j]);
            if (in[j].is_open())
            {
                char temp;
                while (!in[j].eof())
                {
                    //in[j]>>words[k];
                    temp = in[j].get();
                    //cout<<temp<<" ";
                    while (isalpha(temp))
                    {
                        words[k] = words[k] + temp;
                        temp = in[j].get();
                    }
					num[k] = j;
                    //cout<<words[k]<<" "<<j<<"    "<<words[k].empty()<<endl;
                    //index[k].insert(j);
                    //invertedIndex[words[k]] = index[k];
                    //for(int a = 0; a <= k; a++)
                    //{
                        /**
                         * insert first and then pair them up
                         * cannot insert after then.
                         * try conditionally pair them up.
                        */
                        
                        /*if(strcmp(words[k], words[a]) == 0 && a < k)
                        {
                            index[a].insert(j);
                            invertedIndex[words[a]] = index[a];
                        }
                        else if(a == k)
                        {
                            index[a].insert(j);
                            invertedIndex[words[a]] = index[a];
                        }

                    }*/
                    
                    //cout<<invertedIndex.first<<": "<<invertedIndex.second<<endl;
                    //cout<<invertedIndex;
                    
                    k++;
                    
                }
                k--;
                
            }
            in[j].close();
        }
		for(int a = 0; a <= k; a++)
		{
			
			if(words[a].empty() == 1)
			{
				for(int b = a; b <= k; b++)
				{
					words[b] = words[b + 1];
					num[b] = num[b + 1];
				}
				k--;
			}
			//cout<<a<<" "<<words[a]<<" "<<num[a]<<" "<<words[a].empty()<<endl;
		}
		for(int a = 0; a <= k; a++)
		{
			cout<<a<<" "<<words[a]<<" "<<num[a]<<" "<<words[a].empty()<<endl;
			index[a].insert();
            invertedIndex[words[k]] = index[k];
		}
        for (map<string, set<int> >::iterator it = invertedIndex.begin(); it != invertedIndex.end(); ++it)
        {
            cout<<it->first<<":";//<<it->second<<endl
            for (set<int>::iterator setIt = it->second.begin(); setIt != it->second.end(); ++setIt)
                cout<<" "<<*setIt;
            cout<<endl;
        }
    }
    return 0;

}