#include <cstddef> // size_t
#include <iostream> // cout, cin
#include <map> // map
#include <vector> // vector
#include <string> // getline

using namespace std;

static void Histo(void);

int main()
{
    Histo();

    return 0;
}

static void Histo(void)
{
    string sentence;
    vector<string> sentenceOrder;
    map<string, size_t> textMap;

    getline(cin, sentence);

    while ("." != sentence)
    {
        auto sentenceMapIter = textMap.emplace(sentence, 1);

        if (1 != sentenceMapIter.second)
        {
            ++(sentenceMapIter.first->second);
        }

        else
        {
            sentenceOrder.push_back(sentence);
        }

        getline(cin, sentence);
    }

    auto mapRunner = sentenceOrder.begin();
    while(sentenceOrder.end() != mapRunner)
    {
        const string &s = *mapRunner;
        size_t count = textMap[s];

        if (1 == count)
        {
            cout << s << endl;
        }

        else
        {
            cout << count << "X " << s << endl;
        }

        ++mapRunner;
    }
}